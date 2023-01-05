#include <iostream>
#include <cstring>
#include <math.h>
#include <bitset>

struct PrelimInputs{
  	long ramSize;
  	unsigned int cacheSize;
  	unsigned int blockSize;
  	short mapMethod;
  	short replaceMethod;
  	short n;
};

//FUNCTION PROTOTYPES
struct PrelimInputs PrelimInputs();
long toBytes(char []);
std::bitset<32> * generateStorageArray(unsigned int, unsigned int);
void printArray(std::bitset<32> [], unsigned int);
unsigned int inputAddress();
int arrayLength(unsigned int, unsigned int);
int checkCache(unsigned int, std::bitset<32> [], struct PrelimInputs);
void addToCache(unsigned int, struct PrelimInputs, std::bitset<32> []);
short updateExecute();
short checkValidity(unsigned int, unsigned int, unsigned int);
short storageFull(unsigned int, unsigned int, std::bitset<32> *);
unsigned int firstEmptyLine(unsigned int, unsigned int, std::bitset<32> *);
std::bitset<32> combineToTag(std::bitset<32>, std::bitset<32>, short);
unsigned long findMinIndex(std::bitset<32> *, unsigned long, unsigned long, unsigned long);
//END FUNCTION PROTOTYPES

unsigned int g_prio = 0;
int main(){
	short execute = 1, validity = 1;
  	struct PrelimInputs prelimInputs = PrelimInputs();
  	std::bitset<32> * cache = generateStorageArray(prelimInputs.cacheSize,
            prelimInputs.blockSize);
  	unsigned int cacheArrayLength = arrayLength(prelimInputs.cacheSize,
            prelimInputs.blockSize);
	printArray(cache, cacheArrayLength);
	while(execute != 0 & validity != 0){
  		unsigned int address = inputAddress();
		validity = checkValidity(address, 0, prelimInputs.ramSize - 1);
		if (validity != 1){
			std::cout << "Invalid Address, exiting program\n";
			return 0;
		}//if
		checkCache(address, cache, prelimInputs);
		printArray(cache, cacheArrayLength); 
		execute = updateExecute();
	}//while   


	// std::bitset<32> blockNumTest = 6;
	// std::bitset<32> prioTest = 1;
	// short blockLength = 4;
	// std::cout << combineToTag(blockNumTest, prioTest, blockLength);


  	return 0;
}//main()

//Max ram size = 4294967296b = 4gb
struct PrelimInputs PrelimInputs(){
  	struct PrelimInputs prelimInputs;
  	char tempRam[7], tempCache[7], tempBlock[7];
  	long ramBytes, cacheBytes, blockBytes;
  	short mapMethod, replaceMethod, n = 0;

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
    std::cout << "Replacement method: FIFO(1)";
    std::cin >> replaceMethod;

    ramBytes = toBytes(tempRam);
    cacheBytes = toBytes(tempCache);
    blockBytes = toBytes(tempBlock);

    prelimInputs.cacheSize = cacheBytes;
    prelimInputs.ramSize = ramBytes;
    prelimInputs.blockSize = blockBytes;
    prelimInputs.mapMethod = mapMethod;
    prelimInputs.replaceMethod = replaceMethod;
    prelimInputs.n = n;
    return prelimInputs;
}//PrelimInputs()

long toBytes(char input[7]){
    long multiplier;
    char nums[5] = {};
    char alpha[3] = {};
    short numIndex = 0, alphaIndex = 0;

    for(int i = 0; i <= (strlen(input)-1); i++){
        if(isdigit(input[i])){
            nums[numIndex] = input[i];
            numIndex++;
        } else {
            alpha[alphaIndex] = input[i];
            alphaIndex++;
        }//ifelse
    }//for

    int numBytes = atoi(nums);

    if(alpha[0] == 'b'){
        multiplier = pow(2, 0);
    } else if(alpha[0] == 'k'){
        multiplier = pow(2, 10);
    } else if(alpha[0] == 'm'){
        multiplier = pow(2, 20);
    } else if(alpha[0] == 'g'){
        multiplier = pow(2, 30);
    }//ifelseif
    return numBytes * multiplier;
}//toBytes(char[])

std::bitset<32> * generateStorageArray(unsigned int storageSize, unsigned int blockSize){
    unsigned int blockNum = storageSize / blockSize;
    std::bitset<32> * storageArray = (std::bitset<32>*)malloc(sizeof(std::bitset<32>) * (blockNum));
    for(int storageIndex = 0; storageIndex < blockNum; storageIndex++){
      storageArray[storageIndex] = -1;
    }//for
    return storageArray;
}//generateStorageArray(long, long)

void printArray(std::bitset<32> input[], unsigned int length){
    for(int i = 0; i < length; i++){
        std::cout << input[i] << "\n";
    }
	std::cout << "\n";
}//printArray(int[], int)

unsigned int inputAddress(){
  unsigned int address;
  std::cout << "What address would you like to test? \n";
  std::cin >> address;
  return address;
}//inputAddress()

int arrayLength(unsigned int storageSize, unsigned int blockSize){
    int length = storageSize / blockSize;
    return length;
}//arrayLength(unsigned int, unsigned int)

int checkCache(unsigned int address, std::bitset<32> * cache, struct PrelimInputs inputs){
	std::bitset<32> addressBin = address;
	unsigned int numOfLines = inputs.cacheSize/inputs.blockSize;
	unsigned int numOfBlocks = inputs.ramSize/inputs.blockSize;
	unsigned short wordLength = log2(inputs.blockSize);
	unsigned short blockLength = log2(numOfBlocks);
	unsigned short tagLength;

	unsigned int block = address/inputs.blockSize;

    if(inputs.mapMethod == 1){ //direct
		unsigned short lineLength = log2(numOfLines);
		tagLength = (32 - lineLength - wordLength);
		std::bitset<32> tag = addressBin >> (lineLength + wordLength);
		std::bitset<32> line = addressBin << tagLength >> (tagLength + wordLength);
		if(cache[line.to_ulong()] == block){
			std::cout << "cache hit\n";
			return 1;
		}//if
    } else if(inputs.mapMethod == 2){ //associative
		for(int i = 0; i < numOfLines; i++){
			if(cache[i] == block){
				std::cout << "cache hit\n";
				return 1;
			}//if
		}//for
    } else if(inputs.mapMethod == 3){ //set-associative
		unsigned int set = block % inputs.n;
		unsigned int start = inputs.n * set;
		unsigned int end = start + (inputs.n - 1);
		for(int i = start; i <= end; i++){
			if(cache[i] == block){
				std::cout << "cache hit\n";
				return 1;
			}//if
		}//for
	}//ifelseif
	std::cout <<"cache miss\n";
	addToCache(address, inputs, cache);
	return 0;
}//checkCache(unsiged int, int*, struct PrelimInputs)
	
void addToCache(unsigned int address, struct PrelimInputs inputs, std::bitset<32> * cache){
	std::bitset<32> addressBin = address;
	std::bitset<32> blockNum = address/inputs.blockSize;
	std::bitset<32> tag;
	std::bitset<32> line;
	unsigned int numOfLines = inputs.cacheSize/inputs.blockSize;
	short wordLength = log2(inputs.blockSize);
	short tagLength;
	short blockLength = log2(inputs.ramSize/inputs.blockSize);
	
	if(inputs.mapMethod == 1){ //direct
		short lineLength = log2(numOfLines);
		tagLength = 32 - lineLength - wordLength;
		line = addressBin << tagLength >> (tagLength + wordLength);
		cache[line.to_ulong()] = blockNum.to_ulong();
	} else if(inputs.mapMethod == 2){ //associative
		if(storageFull(0, numOfLines, cache)){
			unsigned int minIndex = findMinIndex(cache, 0, numOfLines-1, numOfLines);
			cache[minIndex] = combineToTag(blockNum, (std::bitset<32>)g_prio, blockLength);
			g_prio++;
		} else { 
			cache[firstEmptyLine(0, numOfLines-1, cache)] = combineToTag(blockNum, (std::bitset<32>)g_prio, blockLength);
			//std::cout << "g_prio: " << g_prio << "\n" << "(std::bitset<32>)g_prio: " << (std::bitset<32>)g_prio << "\n";
			g_prio++;
		}
	} else if(inputs.mapMethod == 3){ //set-associative
		// unsigned int set = blockNum % inputs.n;
		// unsigned int start = inputs.n * set;
		// unsigned int end = start + (inputs.n - 1);
		// if(storageFull(start, end, cache) == 0){
			
		// } else {
		// 	//replacement method here
		// }
	}//ifelseif
}//addToCache(unsigned int, struct PrelimInputs, int*)

short updateExecute(){
	short execute;
	std::cout << "Would you like to input another address? Yes(1) or No(0)";
	std::cin >> execute;
	return execute; 
}//updateExecute()

short checkValidity(unsigned int val, unsigned int lower, unsigned int upper){
	if(val < lower | val > upper){
		return 0;
	}
	return 1;
}//checkValidity(unsigned int, unsigned int, unsigned in)

short storageFull(unsigned int start, unsigned int stop, std::bitset<32> * cache){
	for(int i = start; i < stop; i++){
		if(cache[i] == -1){
			return 0;
		}
	}
	return 1;
}//storageFull(unsigned int, unsigned int, int*)

unsigned int firstEmptyLine(unsigned int start, unsigned int stop, std::bitset<32> * cache){
	int i = start;
	while((cache[i] != -1) & (i <= stop)){
		i++;
	}
	return i;
}//firstEmptyLine(int*, unsigned int)

std::bitset<32> combineToTag(std::bitset<32> blockNum, std::bitset<32> prio, short blockLength){
	std::bitset<32> tag;
	for(int i = blockLength, j = 0; j < blockLength; i++, j++){
    tag[i] = prio[j];
    tag[j] = blockNum[j];
  	}
	return tag;
}

unsigned long findMinIndex(std::bitset<32> * cache, unsigned long start, unsigned long end, unsigned long numOfLines){
	unsigned long minIndex = numOfLines-1;
	for(int k = start; k < end; k++){
		if(cache[k].to_ulong() < cache[minIndex].to_ulong()){
			minIndex = k;
		}
	}
	return minIndex;
}


