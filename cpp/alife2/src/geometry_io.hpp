#pragma once
#ifndef _GEOMETRY_IO_H_
#define _GEOMETRY_IO_H_

//namespace std{
//    class ostream;
//};

namespace alife2{
    class vec2;
    class circle;
    class rectangle;
    
    std::ostream & operator << ( std::ostream & os, const vec2 & x );
    std::ostream & operator << ( std::ostream & os, const circle & x );
    std::ostream & operator << ( std::ostream & os, const rectangle & x );
};


#endif /* _GEOMETRY_IO_H_ */
