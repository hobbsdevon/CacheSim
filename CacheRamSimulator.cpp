#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <cstdlib>

struct PrelimInputs{
    long cacheSize;
    long ramSize;
    long blockSize;
    int mapMethod;
    int n;
};

struct Address{
    //int dirtyBit;
    int tag;
    int r;
    int word;
};

//FUNCTION PROTOTYPES
struct PrelimInputs PrelimInputs();
long toBytes(char []);
void printArray(int [], int);
int arrayLength(long, long);
int * generateStorageArray(long, long);
struct Address inputAddress(int, int, int, int, int);
int bitLength(int);
//END FUNCTION PROTOTYPES

int main(){
    struct PrelimInputs prelimInputs = PrelimInputs();
    int * cache = generateStorageArray(prelimInputs.cacheSize,
            prelimInputs.blockSize);
    int cacheArrayLength = arrayLength(prelimInputs.cacheSize,
            prelimInputs.blockSize);
    printArray(cache, cacheArrayLength);
    struct Address address = inputAddress(prelimInputs.ramSize,
            prelimInputs.cacheSize, prelimInputs.blockSize,
            prelimInputs.mapMethod, prelimInputs.n);
    free(cache);
    return 0;
}//main()

//FUNCTIONS
struct PrelimInputs PrelimInputs(){
    struct PrelimInputs prelimInputs;
    char tempRam[7], tempCache[7], tempBlock[7];
    long ramBytes, cacheBytes, blockBytes;
    int mapMethod;
    int n = 1;

    std::cout << "Size of RAM: \n";
    std::cin >> tempRam;
    std::cout << "Size of Cache: \n";
    std::cin >> tempCache;
    std::cout << "Size of Block: \n";
    std::cin >> tempBlock;
    std::cout << "Mapping method: Direct(1) "
            "Associative(2) Set-Associative(3) \n";
    std::cin >> mapMethod;
    if(mapMethod == 3){
        std::cout << "Enter n: (n-way set associative mapping) \n";
        std::cin >> n;
    }

    ramBytes = toBytes(tempRam);
    cacheBytes = toBytes(tempCache);
    blockBytes = toBytes(tempBlock);

    prelimInputs.cacheSize = cacheBytes;
    prelimInputs.ramSize = ramBytes;
    prelimInputs.blockSize = blockBytes;
    prelimInputs.mapMethod = mapMethod;
    prelimInputs.n = n;

    return prelimInputs;
}//PrelimInuts()

long toBytes(char input[7]){
    long multiplier;
    char nums[5] = {};
    char alpha[3] = {};
    int numIndex = 0, alphaIndex = 0;

    for(int i = 0; i <= (strlen(input)-1); i++){
        if(isdigit(input[i])){
            nums[numIndex] = input[i];
            numIndex++;
        } else {
            alpha[alphaIndex] = input[i];
            alphaIndex++;
        }
    }

    int numBytes = atoi(nums);

    if(alpha[0] == 'b'){
        multiplier = pow(2, 0);
    } else if(alpha[0] == 'k'){
        multiplier = pow(2, 10);
    } else if(alpha[0] == 'm'){
        multiplier = pow(2, 20);
    } else if(alpha[0] == 'g'){
        multiplier = pow(2, 30);
    }
    return numBytes * multiplier;
}//toBytes(char[])

int arrayLength(long storageSize, long blockSize){
    int length = storageSize / blockSize;
    return length;
}

int * generateStorageArray(long storageSize, long blockSize){
    int blockNum = storageSize / blockSize;
    int * storageArray = (int*)malloc(sizeof(int) * (blockNum));
    for(int storageIndex = 0; storageIndex < blockNum; storageIndex++){
      storageArray[storageIndex] = -1;
    }//for
    return storageArray;
}//generateCache(long, long)

void printArray(int input[], int length){
    for(int i = 0; i < length; i++){
        std::cout << input[i] << "\n";
    }
    std::cout << "\n";
}//printArray(int[], int)

struct Address inputAddress(int ramSize, int cacheSize, int blockSize,
            int mapMethod, int n){
    struct Address address;
    int tempAddress, tagMask, rMask, wordMask,
            addressLength, tagBitLength, rBitLength, wordBitLength;
    std::cout << "Address you would like to test: \n";
    std::cin >> tempAddress;
    addressLength = bitLength(ramSize);
    //address.dirtyBit = 0;
    int cacheLines = cacheSize/blockSize;
    if(mapMethod == 1){ //direct
        n = 1;
    } else if(mapMethod == 2){ //associative
        n = cacheLines;
    } else if (mapMethod == 3){ //set-associative
        n = n;
    }

    rBitLength = bitLength(cacheLines/n);
    wordBitLength = bitLength(blockSize);
    tagBitLength = addressLength - (rBitLength + wordBitLength);

    wordMask = 0, rMask = 0, tagMask = 0;
    for(int i = 0; i < wordBitLength; i++){
        wordMask += pow(2, i);
    }
    for(int j = wordBitLength; j < (wordBitLength + rBitLength); j++){
        rMask += pow(2, j);
    }
    for(int k = (wordBitLength + rBitLength); k < addressLength; k++){
        tagMask += pow(2, k);
    }

    address.tag = (tempAddress & tagMask) >> (rBitLength + wordBitLength);
    address.r = (tempAddress & rMask) >> (wordBitLength);
    address.word = (tempAddress & wordMask);

    return address;
}//inputAddress(int, int, int, int, int)

int bitLength(int bytes){
    return (log(bytes))/(log(2));
}//bitLength(int)
