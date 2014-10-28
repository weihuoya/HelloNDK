#ifndef __hellosdl__recognizer__
#define __hellosdl__recognizer__

#include "input.h"


class Recognizer
{
public:
    ~Recognizer();

    static Recognizer * instance();

    void recognize(const Input * input);

    void reset();

protected:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Recognizer);

    bool move_;
    bool scale_;
    bool rotate_;
};


#endif /* defined(__HelloSDL__recognizer__) */
