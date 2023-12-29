#ifndef  SAXHandler_h
#define  SAXHandler_h

class SAXHandler
{
  public:
    virtual void OnElementBegin(const char *name) = 0;
    virtual void OnElementEnd  (const char *name) = 0;
    virtual void OnAttrBegin   (const char *name, const char *val) = 0;
    virtual void OnAttrEnd     (const char *name, const char *val) = 0;
    virtual void justVal       (const char *name) = 0;

};

#endif //SAXHandler_h

