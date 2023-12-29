#include <iostream>
#include <string>
#include "Town.h"

//g++ .\Classes.cpp -std=c++98 -pedantic -o .\Classes.exe
int main(){
    Town Moscow, town(5000, "kitten", "Norway", "Prehewill");
    Moscow.Town_info();
    Town Moscow2(Moscow);
    Moscow.change_mayor("C0olH4ck3r");
    Moscow.Town_info();
    Moscow2.Town_info();  
    town.WriteXML(std::cout);
    Moscow.change_num_of_cit(2000000);
    Moscow.Town_info();
    town.Town_info();
    return 0;
}
