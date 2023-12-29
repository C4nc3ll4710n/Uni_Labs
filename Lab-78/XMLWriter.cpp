#include "XMLWriter.h"


XMLWriter::XMLWriter(std::ostream & out)
    : out(out), elements(NULL), numelements(0), inelem(true) {}

void XMLWriter::DocumentBegin(const char *version, const char *encoding){
    out << "<?xml version=\"" << version << "\" encoding=\"" << encoding << "\" ?>" << std::endl;
}

void XMLWriter::DocumentEnd() {}

void XMLWriter::ElementBegin(const char *name)
{
    //std::string *tmp = new std::string[numelements+1];
    //for(size_t i =0; i < numelements; ++i)
    //    tmp[i] = elements[i];
    //tmp[numelements] = name;
    numelements++;
    //delete [] elements;
    //elements = tmp;
  
    for(size_t i = 0; i < numelements; ++i)
        out << "    ";
    out << '<' << elements[numelements-1] ;
  
    //inelem = false;
    //if( inelem == false )
    out << '>' << std::endl; 
    
}

void XMLWriter::ElementBegin(const Element& element){
    for(size_t i = 0; i <= numelements; ++i)
        out << "    ";
    numelements++;
    out << '<'  << element.getName();
    if (element.getAttributes().Size()>0)
        out << " ";
    for (size_t i =0; i< element.getAttributes().Size(); i++){
        Attribute attr = element.getAttributes().getAttribute(i);
        out << attr.getName() << "= \"" << attr.getValue() << "\""; //!something may be wrong
    }
    out  << '>' << std::endl;
    if (element.getVal()!="")
        out << " " << element.getVal() << " ";
}

void XMLWriter::ElementEnd(const Element& elem){
    numelements--;
    for(size_t i = 0; i <= numelements; ++i)
        out << "    ";
    out << "</" << elem.getName() << '>' << std::endl;
    
}

void XMLWriter::WriteElement(const Element& element) {
    for(size_t i = 0; i <= numelements; ++i)
        out << "    ";
    out << '<'  << element.getName();
    if (element.getAttributes().Size()>0)
        out << " ";
    for (size_t i =0; i< element.getAttributes().Size(); i++){
        Attribute attr = element.getAttributes().getAttribute(i);
        out << attr.getName() << "= \"" << attr.getValue() << "\""; //!something may be wrong
    }
    out  << '>';
    out << " " << element.getVal() << " ";
    out << "</" << element.getName() << ">" << std::endl;
}

void XMLWriter::WriteElements(const Element *es, int count){
    for (size_t i=0; i<count; i++)
        WriteElement(es[i]);    
}

void XMLWriter::WriteAttribute(const char *name, const char *value) {
  if( inelem == true ) // was false 
    out << ' '  << name << "=\"" << value << '"' ; //!!!!
}

void XMLWriter::WriteAttribute(const Attribute attr) {
  if( inelem == true ) // was false 
    out << ' '  << attr.getName() << "=\"" << attr.getValue() << '"' ; //!!!!
}

void XMLWriter::WriteAttributes(const AttributeList& attrlst) {
    if (inelem == true){
        for (size_t i = 0; i< attrlst.Size(); i++)
            out << ' '  << attrlst.getAttribute(i).getName() << "=\"" << attrlst.getAttribute(i).getValue() << '"' ;
    }
}

void XMLWriter::WriteAttributes(const Attribute& a1, const Attribute& a2){
    if (inelem == true){
        out << ' '  << a1.getName() << "=\"" << a1.getValue() << '"' ;
        out << ' '  << a2.getName() << "=\"" << a2.getValue() << '"' ;
    }
}

void XMLWriter::WriteVal(const char *value) {
    out << ' ' << value << ' ' ; //!!!!
}


XMLWriter::~XMLWriter(){}

