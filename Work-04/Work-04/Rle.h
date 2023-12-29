#ifndef RLE
#define RLE

class StreamProcessor{
    std::istream& input;
    std::ostream& output;  
    
    public:
        StreamProcessor();
        StreamProcessor(std::istream& input, std::ostream& output);
        void Process();

        
};

#endif //RLE
