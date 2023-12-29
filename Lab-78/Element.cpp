#include "Element.h"

Element::Element(const std::string& name) //конструктор
    : name(name), val(""), attributes(0) {}

Element::Element(const std::string& name, const std::string& val) //конструктор
    : name(name), val(val), attributes(0) {}

const std::string& Element::getName() const{ //геттер имени элемента, const потому что геттер
    return name;
}

const AttributeList& Element::getAttributes() const{ //геттер атрибутов, const потому что геттер
    return attributes;
}

bool  Element::addAttribute(const Attribute& attr){
    return attributes.addAttribute(attr);
}

const std::string& Element::getVal() const{
    return val;
}

Element::~Element(){}

