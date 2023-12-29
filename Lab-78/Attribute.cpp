#include <iostream>
#include "Attribute.h"


Attribute::Attribute() //конструктор умолчания
    :name(""), value("") {}
    
Attribute::Attribute(const Attribute& other) //конструктор копирования, конст потому что не хотим менять ничё
    :name(other.name), value(other.value) {}
   
Attribute::Attribute(const std::string& name) // конструктор с именем
    :name(name) {}
    
Attribute::Attribute(const std::string& name, const std::string& value)
    :name(name), value(value) {}

const std::string& Attribute::getName()  const{ //геттер имени
    return name;
}

const std::string& Attribute::getValue() const{ //геттер значения
    return value;
}
void  Attribute::setValue(const std::string& value) {//сеттер значения
    this->value = value;
}



Attribute::~Attribute(){}

//⠀⠀⠀⠀⠀⠀⢀⡤⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀
//⠀⠀⠀⠀⠀⢀⡏⠀⠀⠈⠳⣄⠀⠀⠀⠀⠀⣀⠴⠋⠉⠉⡆⠀⠀⠀⠀⠀
//⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠈⠉⠉⠙⠓⠚⠁⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀
//⠀⠀⠀⠀⢀⠞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣄⠀⠀⠀⠀
//⠀⠀⠀ ⡞⠀⠀⠀⠀⠀⠶⠀⠀⠀⠀⠀⠀⠦⠀⠀⠀⠀⠀⠸⡆
//⢠⣤⣶⣾⣧⣤⣤⣀⡀⠀⠀⠀⠀⠈⠀⠀⠀⢀⡤⠴⠶⠤⢤⡀⣧⣀⣀⠀
//⠻⠶⣾⠁⠀⠀⠀⠀⠙⣆⠀⠀⠀ ⠀⠀⣰⠋⠀⠀⠀⠀⠀⢹⣿⣭⣽⠇
//⠀⠀⠙⠤⠴⢤⡤⠤⠤⠋⠉⠉⠉⠉⠉⠉⠉⠳⠖⠦⠤⠶⠦⠞⠁⠀⠀
