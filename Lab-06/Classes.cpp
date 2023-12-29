#include <iostream>
#include <fstream>
#include <string>
#include "Town.h"

//g++ .\Classes.cpp -std=c++98 -pedantic -o .\Classes.exe
int main(){
    
    try 
    {
        Town Moscow, town(5000, "kitten", "Norway", "Prehewill");
        std::ifstream inputFile("file.xml");
        Moscow.WriteXML(std::cout);
        Moscow = Moscow.ReadXML(inputFile);
        Moscow.Town_info();
    }
    catch (const char* ex)
    {
        std::cerr << "Caught exception: " << ex << std::endl;
    }
}
