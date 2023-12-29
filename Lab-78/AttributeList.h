#ifndef  AttributeList_h
#define  AttributeList_h

#include "Attribute.h"

class AttributeList
{
    Attribute  *list ; // список атрибутов
    size_t      count; // количество атрибутов
    size_t      maxsize;  // максимальное число атрибутов
    
  public:
    AttributeList(size_t size); // конструктор с размером
    
    const Attribute& getAttribute(size_t i) const; //геттер(?) атрибута
    
    bool  addAttribute(const Attribute& attr);
    
    size_t Size() const; // размер листа

    void printList(const AttributeList& attrlst);
};

#endif //AttributeList_h

