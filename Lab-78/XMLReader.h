#ifndef  XMLReader_h
#define  XMLReader_h

#include <istream>
#include "SAXHandler.h"
#include "Element.h"

class XMLReader
{
    std::istream & in;
    SAXHandler &handler;
    
  public:
    void OnElementBegin(const char *name);
    void OnElementEnd  (const char *name);
    XMLReader(std::istream &in, SAXHandler &handler); //! тут наследуется!!
    void ParseXML();
};



#endif //XMLReader_h

