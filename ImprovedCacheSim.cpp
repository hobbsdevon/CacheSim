#include <iostream>
#include <vector>
#include <bitset>
#include <list>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <limits>

using namespace std;

struct CacheLine {
    bitset<32> tag;
    bool valid = false;
    unsigned int priority = 0;
};

class CacheSimulator {
private:
    uint64_t ramSize;
    uint32_t cacheSize, blockSize;
    int mappingMethod; // 1: direct, 2: fully associative, 3: set-associative
    int replaceMethod; // 1: FIFO, 2: LRU
    int associativity;
    int numSets;
    int numLines;
    uint32_t offsetBits;
    uint32_t indexBits;
    vector<vector<CacheLine>> cache;
    int hits = 0;
    int misses = 0;
    vector<unsigned int> fifoCounters;
    vector<list<int>> lruOrders;
    vector<unordered_map<int, list<int>::iterator>> lruPositions;

    int findEmpty(int set) const {
        for (int i = 0; i < associativity; ++i) {
            if (!cache[set][i].valid) {
                return i;
            }
        }
        return -1;
    }

    int findMinPrio(int set) const {
        unsigned int minPrio = numeric_limits<unsigned int>::max();
        int idx = -1;
        for (int i = 0; i < associativity; ++i) {
            if (cache[set][i].valid && cache[set][i].priority < minPrio) {
                minPrio = cache[set][i].priority;
                idx = i;
            }
        }
        return idx;
    }

public:
    CacheSimulator(uint64_t r, uint32_t c, uint32_t b, int m, int rep, int a)
        : ramSize(r), cacheSize(c), blockSize(b), mappingMethod(m), replaceMethod(rep), associativity(a) {
        numLines = cacheSize / blockSize;
        if (mappingMethod == 1) {
            numSets = numLines;
        } else if (mappingMethod == 2) {
            numSets = 1;
            associativity = numLines;
        } else {
            numSets = numLines / associativity;
        }
        cache.resize(numSets, vector<CacheLine>(associativity));
        fifoCounters.assign(numSets, 0);
        if (replaceMethod == 2) {
            lruOrders.resize(numSets);
            lruPositions.resize(numSets);
        }
        offsetBits = static_cast<uint32_t>(log2(blockSize));
        indexBits = static_cast<uint32_t>(log2(static_cast<double>(numSets)));
    }

    bool access(uint32_t address) {
        uint32_t lineAddr = address / blockSize;
        int set = 0;
        uint32_t tag_val;
        if (mappingMethod == 2) {
            set = 0;
            tag_val = lineAddr;
        } else {
            set = lineAddr % numSets;
            tag_val = lineAddr / numSets;
        }
        bitset<32> tag(tag_val);

        auto& setCache = cache[set];
        bool hit = false;
        int hitIdx = -1;
        for (int i = 0; i < associativity; ++i) {
            if (setCache[i].valid && setCache[i].tag == tag) {
                hit = true;
                hitIdx = i;
                break;
            }
        }

        if (hit) {
            cout << "cache hit" << endl;
            ++hits;
            if (replaceMethod == 2) {
                auto& order = lruOrders[set];
                auto& posmap = lruPositions[set];
                order.erase(posmap[hitIdx]);
                order.push_back(hitIdx);
                posmap[hitIdx] = --order.end();
            }
        } else {
            cout << "cache miss" << endl;
            ++misses;
            int victimIdx = findEmpty(set);
            bool full = (victimIdx == -1);
            if (full) {
                if (replaceMethod == 1) {
                    victimIdx = findMinPrio(set);
                } else {
                    auto& order = lruOrders[set];
                    int lruIdx = order.front();
                    order.pop_front();
                    lruPositions[set].erase(lruIdx);
                    victimIdx = lruIdx;
                }
            }
            setCache[victimIdx].tag = tag;
            setCache[victimIdx].valid = true;
            if (replaceMethod == 1) {
                setCache[victimIdx].priority = fifoCounters[set]++;
            } else {
                auto& order = lruOrders[set];
                order.push_back(victimIdx);
                lruPositions[set][victimIdx] = --order.end();
            }
        }
        return hit;
    }

    void printCache() const {
        cout << "Cache contents:" << endl;
        for (int s = 0; s < numSets; ++s) {
            cout << "Set " << s << ":" << endl;
            for (int i = 0; i < associativity; ++i) {
                const auto& line = cache[s][i];
                cout << "  Line " << (s * associativity + i) << ": valid=" << line.valid
                     << ", tag=0x" << hex << line.tag.to_ulong() << dec << endl;
            }
        }
    }

    double getHitRate() const {
        int total = hits + misses;
        return (total > 0) ? static_cast<double>(hits) * 100.0 / total : 0.0;
    }
};

uint64_t toBytes(const string& input) {
    istringstream iss(input);
    unsigned long long num = 0;
    char suffix = '\0';
    iss >> num;
    if (iss >> suffix) {
        suffix = toupper(suffix);
        if (suffix == 'K') {
            num <<= 10;
        } else if (suffix == 'M') {
            num <<= 20;
        } else if (suffix == 'G') {
            num <<= 30;
        } else {
            cout << "Invalid suffix in input: " << input << endl;
            return 0;
        }
    }
    if (num == 0 || iss.fail()) {
        cout << "Invalid number in input: " << input << endl;
        return 0;
    }
    return num;
}

int main() {
    uint64_t ramSize;
    string inputStr;
    bool valid = false;

    // Read RAM size
    while (!valid) {
        cout << "Enter RAM size (e.g., 1G): ";
        cin >> inputStr;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        ramSize = toBytes(inputStr);
        if (ramSize > 0) {
            valid = true;
        } else {
            cout << "Please enter a valid RAM size." << endl;
        }
    }

    // Read cache size
    uint32_t cacheSize;
    valid = false;
    while (!valid) {
        cout << "Enter cache size (e.g., 1M): ";
        cin >> inputStr;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        uint64_t temp = toBytes(inputStr);
        if (temp > UINT32_MAX || temp == 0) {
            cout << "Cache size must be a valid 32-bit value." << endl;
            continue;
        }
        cacheSize = static_cast<uint32_t>(temp);
        valid = true;
    }

    // Read block size
    uint32_t blockSize;
    valid = false;
    while (!valid) {
        cout << "Enter block size (e.g., 64): ";
        cin >> inputStr;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        uint64_t temp = toBytes(inputStr);
        if (temp > UINT32_MAX || temp == 0 || temp > cacheSize) {
            cout << "Block size must be valid and <= cache size." << endl;
            continue;
        }
        blockSize = static_cast<uint32_t>(temp);
        valid = true;
    }

    if (cacheSize % blockSize != 0) {
        cout << "Cache size must be multiple of block size." << endl;
        return 1;
    }

    uint32_t numLines = cacheSize / blockSize;
    if (numLines == 0) {
        cout << "Invalid configuration: no cache lines." << endl;
        return 1;
    }

    // Read mapping method
    int mappingMethod;
    valid = false;
    while (!valid) {
        cout << "Mapping method (1: direct, 2: fully associative, 3: set-associative): ";
        cin >> mappingMethod;
        if (cin.fail() || mappingMethod < 1 || mappingMethod > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter 1, 2, or 3." << endl;
            continue;
        }
        valid = true;
    }

    int associativity = 1;
    if (mappingMethod == 3) {
        valid = false;
        while (!valid) {
            cout << "Enter associativity n (1 < n < " << numLines << "): ";
            cin >> associativity;
            if (cin.fail() || associativity < 2 || associativity >= numLines || numLines % associativity != 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid associativity." << endl;
                continue;
            }
            valid = true;
        }
    } else if (mappingMethod == 2) {
        associativity = numLines;
    }

    // Read replacement method
    int replaceMethod;
    valid = false;
    while (!valid) {
        cout << "Replacement method (1: FIFO, 2: LRU): ";
        cin >> replaceMethod;
        if (cin.fail() || replaceMethod < 1 || replaceMethod > 2) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter 1 or 2." << endl;
            continue;
        }
        valid = true;
    }

    CacheSimulator sim(ramSize, cacheSize, blockSize, mappingMethod, replaceMethod, associativity);

    cout << "Enter memory addresses (hex with 0x or decimal, non-number to stop):" << endl;

    while (true) {
        string addrStr;
        cout << "Address: ";
        cin >> addrStr;
        if (cin.fail() || addrStr.empty()) {
            break;
        }
        istringstream iss(addrStr);
        uint32_t addr;
        iss >> hex >> addr;
        if (iss.fail()) {
            cout << "Invalid address: " << addrStr << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        sim.access(addr);
        sim.printCache();
        cout << "Current hit rate: " << fixed << setprecision(2) << sim.getHitRate() << "%" << endl;
    }

    cout << "Final hit rate: " << fixed << setprecision(2) << sim.getHitRate() << "%" << endl;

    return 0;
}