#include <iostream>
#include <cstdio>
#include "Rle.h"


StreamProcessor::StreamProcessor()
    : input(std::cin), output(std::cout) {}
   
StreamProcessor::StreamProcessor(std::istream& in, std::ostream& out)
    : input(in), output(out) {}
    
void StreamProcessor::Process(){
    char c = 0;
    int count = 1;
    c = input.get();
    while (input.peek()!=EOF){
        
        if (c == input.peek()){
            input.get();
            count++;
        }
        else if (count == 1){
            c = input.get();
            output.put(1);
            output.put(c);
            
        }
        else if (count > 127){
            std::cout << count << "\n";
            output.put(255);
            output.put(count-127);
            output.put(c);
            count = 1;
            
        }
        else{
            output.put(count & 0xff);
            output.put(c);
            count = 1;
            c = input.get();
        }
            
    }
    if (count == 1){
            if (c!=EOF){
                output.put(1);
                output.put(c);
                std::cout << "this is c" << c << "\n";
            }
            
        }
        else if (count > 127){
            std::cout << count << "\n";
            output.put(255);
            output.put(count-127);
            output.put(c);
            count = 1;
            
        }
        else{
            output.put(count & 0xff);
            output.put(c);
            count = 1;
            c = input.get();
        }
} 



