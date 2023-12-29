#include "Town.h"
#include <iostream>
#include <string>
Town::Town(){ //constructor
    number_of_citizens = 1000000;
    mayor_name = "Sobyanin";
    country_name = "Russia";
    town_name = "Moscow";
}
Town::Town(int num_cit, std::string mayor, std::string country, std::string town) //second constructor
    : number_of_citizens(num_cit), mayor_name(mayor), country_name(country), town_name(town) {}

Town::Town(const Town &to_copy){ //copy constructor
    if (to_copy.town_name != "") town_name = to_copy.town_name; 
    if (to_copy.mayor_name != "") mayor_name = to_copy.mayor_name; 
    if (to_copy.country_name != "") country_name = to_copy.country_name;
    number_of_citizens = to_copy.number_of_citizens;
}        
void Town::change_mayor(std::string new_mayor_name){
    this -> mayor_name = new_mayor_name;
}
void Town::change_num_of_cit(int new_num){
    this -> number_of_citizens = new_num;
}

int Town::howmanycit() { return number_of_citizens; }

std::string& Town::whoismayor() { return mayor_name; }

std::string& Town::whatcountry() { return country_name; }

std::string& Town::whattown() { return town_name; }

void Town::Town_info(){
    std::cout << "--------------------------------------------------------------------\n"; 
    std::cout << "Country: " << country_name << "\n";
    std::cout << "Town name: " << town_name << "\n";
    std::cout << "Mayor: " << mayor_name << "\n";
    std::cout << "Number of citizens: " << number_of_citizens << "\n";
    std::cout << "--------------------------------------------------------------------\n"; 
}

void Town::WriteXML(std::ostream& o){
    o << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
    o << "  <NumberOfCitizens =\" " << howmanycit() << "\" />" << std::endl;
    o << "  <MayorName =\" " << whoismayor() << " \" />" << std::endl;
    o << "  <CountryName =\" " << whatcountry() << " \" />" << std::endl;
    o << "  <TownName =\" " << whattown() << " \" />" << std::endl;
}

Town::~Town(){} //destrusctor

