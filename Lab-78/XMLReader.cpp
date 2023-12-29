#include "XMLReader.h"

#include <string>
#include <iostream>

bool isVer(const std::string& str) {
    bool isVersion = true;

    for (size_t i =0; i<str.size(); i++){
        if ((str[i]<'0' || str[i]>'9') && (str[i]!='.' && str[i]!='"'))
            isVersion = false;
    }

    return isVersion;
}

XMLReader::XMLReader(std::istream &in, SAXHandler &handler)
  : in(in), handler(handler) {}
// g++ .\main.cpp .\attributelist.cpp .\attribute.cpp .\element.cpp .\XMLWriter.cpp .\XMLReader.cpp   -o prog.exe
void XMLReader::ParseXML() {
    if( in.get() == '<' ){
        
        if( in.get() == '?' ){
            std::string str;
            in >> str;
            if( str == "xml" ){
                in >> str;
                
                if( str.substr(0, 9) == "version=\"" && (isVer(str.substr(8, 11)) && str[12]=='"') ){
                    in >> str;
                    
          
                    if( str == "encoding=\"UTF-8\"" ){
                        in >> str;
            
                        if( str == "?>" ){
	      
	                          if( in.peek() == '\n' ) in.get();
	                          while( in >> str ) {
	                              if(str[0] == '<'){
	                                  size_t end = str.find(' ');
		                                if( end == std::string::npos ) 
                                        end =  str.find('>');
                                      
                                    //20231203L
		                                if( str[1] == '/' )
		                                    handler.OnElementEnd(str.substr(2,end-1).c_str());
		                                else{
		                                    handler.OnElementBegin(str.substr(1,end-1).c_str());
                                        
                                    }
	                              }
                                else {
                                    if (str.find('=')!=std::string::npos){
                                        size_t beg_attr_nm = 0;
                                        // <student class="mage"  level="cool" element="Hydro">
                                        size_t end_attr_nm = str.find('=', beg_attr_nm);  
                                        size_t beg_attr_val = end_attr_nm+1;
                                        size_t end_attr_val = str.size()-1;
                                        if (str[end_attr_val] == '>') 
                                            end_attr_val -=1;
                                        handler.OnAttrBegin(str.substr(beg_attr_nm, end_attr_nm).c_str(), str.substr(beg_attr_val, end_attr_val- beg_attr_val+1).c_str());
                                    }
                                    else
                                        handler.justVal(str.c_str());
                                }
	                          }
	                          /* ---- */
	                      }
                        else 
                            throw std::runtime_error("Close tag error\n");
                    }
                    else 
                        throw std::runtime_error("Encoding error\n");
                }
                else
                    throw std::runtime_error("Version error\n");
            }
        }
    }
}

