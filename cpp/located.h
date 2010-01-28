#ifndef LOCATED_H_INCLUDED
#define LOCATED_H_INCLUDED
#include "vec2.h"

class Located
{
    public:
        virtual const vec2& get_location()=0;
        virtual ~Located(){};

};
#endif
