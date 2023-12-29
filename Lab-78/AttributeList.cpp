#include "Attribute.h"
#include "AttributeList.h"

AttributeList::AttributeList(size_t size) // конструктор с размером
    : list(NULL), count(0), maxsize(size)
{
    list = new Attribute[maxsize]; //!new?
}

const Attribute& AttributeList::getAttribute(size_t i) const{ //геттер(?) атрибута
    return list[i];
}

bool  AttributeList::addAttribute(const Attribute& attr) {
  if (count < maxsize){
    list[count++] = attr;
    return true;
  }
  //?тут можно вставить вывод ошибки
  return false;
}

size_t AttributeList::Size() const{ // размер листа
    return count;
}