#include <iostream>
#include <string>
#include <cctype>
#include <cmath>

struct FourInputs{
    double cacheSize;
    double ramSize;
    double blockSize;
    int mapMethod;
};

struct FourInputs * PrelimInputs();
long toBytes(char input[7]);
void printTest();

int main(){
    //PrelimInputs();
    char test[] = "512mb";
    std::cout << toBytes(test);
    return 0;
}//main()

//FUNCTIONS
/*
struct FourInputs * PrelimInputs(){
    char tempRam[7], tempCache[7], tempBlock[7];
    int mapMethod;

    std::cout << "Size of RAM: \n";
    std::cin >> tempRam;
    std::cout << "Size of Cache: \n";
    std::cin >> tempCache;
    std::cout << "Size of Block: \n";
    std::cin >> tempBlock;
    std::cout << "Mapping method: Direct(1) "
            "Associative(2) Set-Associative(2) \n";
    std::cin >> mapMethod;

    double ramBytes = toBytes(tempRam);
    double cacheBytes = toBytes(tempCache);
    double blockBytes = toBytes(tempBlock);

    return &prelimInputs;
}//PrelimInuts()
*/

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
    bytes = numBytes * multiplier;
    return bytes;
}//toBytes

void printTest(){

}//printTest()
