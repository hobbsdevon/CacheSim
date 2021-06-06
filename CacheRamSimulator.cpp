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

//FUNCTION PROTOTYPES
struct PrelimInputs * PrelimInputs();
long toBytes(char input[]);
void printArray(int input[], int length);
int arrayLength(long size1, long size2);
int * generateCache(long cacheSize, long blockSize);
//END FUNCTION PROTOTYPES

int main(){
    struct PrelimInputs * prelimInputs = PrelimInputs();
    int * cache = generateCache(prelimInputs -> cacheSize,
            prelimInputs -> blockSize);
    int cacheArrayLength = arrayLength(prelimInputs -> cacheSize,
            prelimInputs -> blockSize);
    printArray(cache, cacheArrayLength);
    free(cache);
    return 0;
}//main()

//FUNCTIONS
struct PrelimInputs * PrelimInputs(){
    struct PrelimInputs prelimInputs;
    char tempRam[7], tempCache[7], tempBlock[7];
    long ramBytes, cacheBytes, blockBytes;
    int mapMethod;
    int n = 0;

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

    return &prelimInputs;
}//PrelimInuts()


long toBytes(char input[7]){
    long bytes, multiplier;
    char nums[5];
    char alpha[3];
    int numIndex = 0;
    int alphaIndex = 0;
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

int arrayLength(long size1, long size2){
    int length = size1 / size2;
    return length;
}

int * generateCache(long cacheSize, long blockSize){
    int cacheLines = cacheSize / blockSize;
    int * cache = (int*)malloc(sizeof(int) * cacheLines);
    for(int cacheIndex = 0; cacheIndex < cacheLines+1; cacheIndex++){
      cache[cacheIndex] = -1;
    }//for
    return cache;
}//generateCache(long, long)

void printArray(int input[], int length){
    for(int i = 0; i < length + 1; i++){
        std::cout << input[i] << "\n";
    }
}//printTest()
