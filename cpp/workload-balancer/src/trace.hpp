#ifndef _TRACE_H_
#define _TRACE_H_

#ifndef DO_TRACE
#    define TRACE( X ) ;
#else
#    include <iostream>
#    ifndef TRACE_NAME
#        define TRACE_NAME "[TRC]"
#    endif
#    define TRACE( X ) { std::cout<< TRACE_NAME << X << std::endl; std::cout.flush(); };
#endif


#endif /* _TRACE_H_ */
