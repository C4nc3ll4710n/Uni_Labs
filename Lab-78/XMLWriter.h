#ifndef  XMLWriter_h
#define  XMLWriter_h

#include "Element.h"
#include <ostream>
#include <string>

class XMLWriter {
    std::ostream & out;
    std::string   *elements;
    size_t      numelements;
    bool             inelem;

    public:
    XMLWriter(std::ostream & out);
    void DocumentBegin(const char *version, const char *encoding); //*
    void DocumentEnd(); //*
    void ElementBegin(const char *name); //группа перегруженных методов для записи тега, в том числе с аттрибутами
    void ElementBegin(const Element& elem); //группа перегруженных методов для записи тега, в том числе с аттрибутами
    void ElementEnd(const Element& elem); //*
    void WriteAttribute(const char *name, const char *value);//*
    void WriteAttribute(const Attribute attr); //*
    void WriteAttributes(const AttributeList& attrlst); //группа перегруженных методов для записи аттрибутов
    void WriteAttributes(const Attribute &a1, const Attribute &a2);
    void WriteElement(const Element& element); //*
    void WriteElements(const Element *es, int count);
    void WriteVal(const char *value);
    ~XMLWriter();
};
#endif // XMLWriter_h
