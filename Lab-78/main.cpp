#include "XMLWriter.h"
#include "XMLReader.h"

#include <fstream>
#include <iostream>


class Handler: public SAXHandler
{
  public:
    void OnElementBegin (const char *name) {
      std::cout << "Found element: " << name << std::endl;
    }
    void OnElementEnd   (const char *name) {}
    void OnAttrBegin    (const char *name, const char *val) {
        std::cout << "Attribute of element: " << name << "=" << val << std::endl;
    }
    void OnAttrEnd      (const char *name, const char *val){}
    void justVal(const char *name) {
        std::cout << "Value of element:" << name << std::endl;
    }
};

int main() {

    try {
	    std::ifstream in("file.xml");
	    if( in.is_open() ){
	        Handler   h;
	        XMLReader xml(in,h);
	        xml.ParseXML();
	    }
	    std::ofstream out("fileout.xml");
        Element name("name", "Serj"), student("student"), group("group", "CoSec-2023");
        student.addAttribute(Attribute("class", "mage"));
        student.addAttribute(Attribute("element","Hydro"));
        student.addAttribute(Attribute());
        if( out.is_open() ){
	        XMLWriter xml(out);
	        xml.DocumentBegin("1.1", "UTF-8");
	        xml.ElementBegin(student);
	        xml.WriteElement(name);
	        xml.WriteElement(group);
	        xml.ElementEnd(student);
	        xml.DocumentEnd();
	        out.close();
	    }
    }
    catch (std::exception &e) {
        std::cerr << "Cought exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Cought something..." << std::endl;
    }
}
