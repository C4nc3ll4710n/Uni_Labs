#ifndef  Attribute_h
#define  Attribute_h

#include <string>

class Attribute {
    std::string name; //имя аттрибута
    std::string value; //его значение
    
  public:
    Attribute(); //конструктор умолчания
    Attribute(const Attribute& other); //конструктор копирования, конст потому что не хотим менять ничё
   
    Attribute(const std::string& name); // конструктор с именем
    Attribute(const std::string& name, const std::string& value); // конструктор с именем

    const std::string& getName()  const; //геттер имени
    const std::string& getValue() const; //геттер значения
    void  setValue(const std::string& value); //сеттер значения

    ~Attribute(); //деструктор
};

#endif //Attribute_h
