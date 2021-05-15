/*
Cache simulation program: by Devon Hobbs
-----------------------------------------------------------------------------
One time user input size of RAM, cache, and block, as well as mapping method.
User inputs address which they would like to check, and program shows cache
hit or miss, and loads that block into the cache should it be a miss.
User can input multiple addresses.
Keeps updated cache while program is running.
-----------------------------------------------------------------------------
last updated: 11 May 2021
*/
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

int toDecimal(int binary){
    int num = binary;
    int decimal = 0;

    int base = 1;
    int temp = num;
    while (temp) {
        int lastDigit = temp % 10;
        temp = temp / 10;
        decimal += lastDigit * base;
        base = base * 2;
    }//while
     return decimal;
}//toDecimal

//Block in RAM that given address is in
int blockNum(int address, int blockSize){
  int block = address/blockSize;
  return block;
}//blockNum

//Returns number of bits needed to store given input
int bitLength(int input){
  return log10(input)/log10(2); //log2(input);
}//bitLength

/*
Returns given input into bytes
Works on value which is up to 4 integers followed by b, kb, mb, or gb
*/
long toBytes(char input[6]){
  char nums[4] = {};
  char alpha[2] = {};
  int numsIndex = 0;
  int alphaIndex = 0;
  for(int i = 0; i < (strlen(input)-1); i++){
    if(isdigit(input[i])){
      memmove(&nums[numsIndex], &input[i], 1);
      numsIndex++;
    } else {
      memmove(&alpha[alphaIndex], &input[i], 1);
      alphaIndex++;
    }//if else
  }//for
  int numsInt = atoi(nums);
  if(alpha[0] == 'k'){
    return numsInt*(long)(pow(2, 10));
  } else if(alpha[0] == 'm'){
    return numsInt*(long)(pow(2,20));
  } else if(alpha[0] == 'g'){
    return numsInt*(long)(pow(2,30));
  }//if else
  return numsInt;
}//toBytes

/*
Generates empty cache array. I had to use pointer and malloc to make the
array accessible outside the function without being global in scope as
well as variably sized
*/
int * generateCache(int cacheSize){
  int * cache = malloc(sizeof(int) * cacheSize);

  for(int cacheIndex = 0; cacheIndex < cacheSize; cacheIndex++){
    cache[cacheIndex] = -1;
  }//for
  return cache;
}//generateCache

int toBinary(int decimalNum){
  int binaryNum = 0;
  int rem, temp = 1;
  while(decimalNum != 0){
    rem = decimalNum % 2;
    decimalNum = decimalNum / 2;
    binaryNum = binaryNum + rem * temp;
    temp = temp * 10;
  }//while
  return binaryNum;
}//toBinary

//Generates tag part of address
int tag(int block, int cacheSize){
  int binaryBlock = toBinary(block);
  int tagBitNum = bitLength(cacheSize);
  int tag = binaryBlock/(pow(10, tagBitNum));
  return tag;
}//tag

/*
Generates following part of address (set if set-associative, line if direct)
toGetR is named this because depending on your mapping method, the value
used to find r is different
  if set-associative, toGetR is number of numOfSets
  if direct, toGetR is cacheSize, or number of lines
*/
int r(int block, int toGetR){
  int binaryBlock = toBinary(block);
  int rBitNum = bitLength(toGetR);
  int r = binaryBlock % (int)(pow(10, rBitNum));
  return r;
}//r

/*
Following input functions are written like this because I didn't have time
to focus on how to return multiple values from a single function with pointers.
That would be one of my next changes, to incorporate the one-time user input
into a single function call.
Input is converted to bytes using toBytes() within input function.
*/
long ramSizeInput(){
  char ramSize[6];
  printf("Size of RAM: \n");
  scanf("%s", ramSize);
  long ramBytes = toBytes(ramSize);
  return ramBytes;
}//ramSizeInput

long cacheSizeInput(){
  char cacheSize[6];
  printf("Size of cache: \n");
  scanf("%s", cacheSize);
  long cacheBytes = toBytes(cacheSize);
  return cacheBytes;
}//cacheSizeInput

long blockSizeInput(){
  char blockSize[6];
  printf("Size of block: \n");
  scanf("%s", blockSize);
  long blockBytes = toBytes(blockSize);
  return blockBytes;
}//blockSizeInput

int mapMethodInput(){
  int mapMethod;
  printf("Mapping Method (direct (1), associative (2), or set-associative (3)): \n");
  scanf("%d", &mapMethod);
  return mapMethod;
}//mapMethodInput

//n-way mapping if mapping method is associative
int nInput(){
  int n;
  printf("Input n (n-way associative mapping(0 if not applicable)): \n");
  scanf("%d", &n);
  return n;
}//nInput

int inputAddress(){
  int address;
  printf("Address you would like to test: \n");
  scanf("%d", &address);
  return address;
}//inputAddress

//adds block tag to cache array
void addToCache(int block, int cache[], int mapMethod, int cacheSize, int n){ //tag as a parameter
  int addressTag = tag(block, cacheSize);
  if(mapMethod == 1){
    int line = block%cacheSize;
    cache[line] = addressTag;
  } else if(mapMethod == 2){
    int randomIndex = rand() % (cacheSize + 1);
    cache[randomIndex] = addressTag;
  } else {
    int randomIndex = rand() % (n + 1);
    int numOfSets = cacheSize/n;
    cache[randomIndex + toDecimal(r(block, numOfSets))] = addressTag;
  }//if else
}//addToCache

//checks array for tag at correct cache line and prints hit or miss
//adds tag to cache if it was a miss
int checkCache(int block, int cache[], int mapMethod, int cacheSize, int n){
  int addressTag = tag(block, cacheSize);
  if(mapMethod == 1){
    int line = block%cacheSize;
    if(cache[line] == addressTag){
      printf("\ncache hit\n");
      return 1;
    }//if
  } else if(mapMethod == 2) {


    for(int i = 0; i < cacheSize; i++){
      if(cache[i] == addressTag){
        printf("\ncache hit\n");
        return 1;
      }//if
    }//for
  } else {
    int numOfSets = cacheSize/n;
    int start = toDecimal(r(block, numOfSets));
    int end = start + n;
    for(int i = start; i < end; i++){
      if(cache[i] == addressTag){
        printf("\ncache hit\n");
        return 1;
      }//if
    }//for
  }//if else
  addToCache(block, cache, mapMethod, cacheSize, n);
  printf("\ncache miss\n");
  return 0;
}//checkCache

//run after every cache update to see if user wants to input another address
int updateExecute(int execute){
  printf("Would you like to test another address? Yes:(1) No:(0)");
  scanf("%d", &execute);
  return execute;
}

int checkValidity(int min, int max, int val){
  if(val < min || val > max){
    return 0;
  }
  return 1;
}

//main method
int main(){
  int execute = 1;
  int n;
  int ramSize = ramSizeInput();
  int cacheSize = cacheSizeInput();
  int blockSize = blockSizeInput();
  int mapMethod = mapMethodInput();
  if(mapMethod == 3){
    n = nInput();
  } else if(mapMethod == 2){
    n = cacheSize;
  } else {
    n = 1;
  }//if else
  int * cache = generateCache(cacheSize);
  for(int i = 0; i < cacheSize; i++){
    printf("%d ", cache[i]);
  }//for
  printf("\n");
  while(execute != 0){
    int block = blockNum(inputAddress(), blockSize);
    checkCache(block, cache, mapMethod, cacheSize, n);
    for(int i = 0; i < cacheSize; i++){
      printf("%d ", cache[i]);
    }//for
    printf("\n");
    execute = updateExecute(execute);
  }//while
  free(cache); //used to free up space previously allocated to cache array by malloc
  return 0;
}//main
