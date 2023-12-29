#include <iostream>
#include <cstdio>
#include <fstream>
#include "Rle.h"

int main(){
    std::ofstream outputFile("output.txt");
    if (!outputFile){
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }
    StreamProcessor sp(std::cin, outputFile);
   
    
    sp.Process();
    
    outputFile.close();
    return 0;
}
