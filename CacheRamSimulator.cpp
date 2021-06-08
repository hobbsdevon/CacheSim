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
    int dirtyBit;
    int tag;
    int r;
    int word;
};

//FUNCTION PROTOTYPES
struct PrelimInputs PrelimInputs();
long toBytes(char input[]);
void printArray(int input[], int length);
int arrayLength(long storageSize, long blockSize);
int * generateStorageArray(long storageSize, long blockSize);
struct Address inputAddress(int ramSize, int mapMethod, int cachelines, int n);
int bitLength(int bytes);
//END FUNCTION PROTOTYPES

int main(){
    std::cout << bitLength(4) << "\n";
    std::cout << bitLength(128) << "\n";
    std::cout << bitLength(1024) << "\n";
    struct PrelimInputs prelimInputs = PrelimInputs();
    int * cache = generateStorageArray(prelimInputs.cacheSize,
            prelimInputs.blockSize);
    int cacheArrayLength = arrayLength(prelimInputs.cacheSize,
            prelimInputs.blockSize);
    printArray(cache, cacheArrayLength);
    free(cache);
    return 0;
}//main()

//FUNCTIONS
struct PrelimInputs PrelimInputs(){
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

    return prelimInputs;
}//PrelimInuts()


long toBytes(char input[7]){
    long bytes, multiplier;
    char nums[5] = {};
    char alpha[3] = {};
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

int arrayLength(long storageSize, long blockSize){
    int length = storageSize / blockSize;
    return length;
}

int * generateStorageArray(long storageSize, long blockSize){
    int blockNum = storageSize / blockSize;
    int * storageArray = (int*)malloc(sizeof(int) * blockNum);
    for(int storageIndex = 0; storageIndex < blockNum+1; storageIndex++){
      storageArray[storageIndex] = -1;
    }//for
    return storageArray;
}//generateCache(long, long)

void printArray(int input[], int length){
    for(int i = 0; i < length + 1; i++){
        std::cout << input[i] << "\n";
    }

}//printArray(int[], int)


struct Address inputAddress(int ramSize, int mapMethod, int cacheLines, int n){
    struct Address address;
    int tempAddress;
    int addressLength = bitLength(ramSize);
    std::cout << "Address you would like to test: \n";
    std::cin >> tempAddress;
    address.dirtyBit = 0;
    if(mapMethod == 0){ //direct
        n = 1;
        address.tag = tempAddress << ();
        address.r = tempAddress << ();
        address.word = tempAddress << ();
    } else if(mapMethod == 1){ //associative
        n = cacheLines;
        address.tag = tempAddress << ();
        address.r = tempAddress << ();
        address.word = tempAddress << ();
    } else if (mapMethod == 2){ //set-associative
        n = n;
        address.tag = tempAddress << ();
        address.r = tempAddress << ();
        address.word = tempAddress << ();
    }
    return address;
}//inputAddress(int)


int bitLength(int bytes){
    return (log(bytes))/(log(2));
}//bitLength(int)
