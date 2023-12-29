#ifndef  Element_h
#define  Element_h

#include <string>
#include "AttributeList.h"

class Element{
    std::string   name; 
    std::string   val;
    AttributeList attributes; //список аттрибутов
  
  public:
    Element(const std::string& name); //конструктор
    Element(const std::string& name, const std::string& val); //конструктор
    const std::string& getName() const; //геттер имени элемента, const потому что геттер
    const std::string& getVal() const; //геттер имени элемента, const потому что геттер
    const AttributeList& getAttributes() const; //геттер атрибутов, const потому что геттер
    bool  addAttribute(const Attribute& attr); 
    ~Element(); //деструктор
};

#endif //Element_h
