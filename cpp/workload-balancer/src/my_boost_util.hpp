#ifndef _MY_BOOST_UTIL_H_
#define _MY_BOOST_UTIL_H_


template< class iter_t >
size_t range_size( iter_t begin, iter_t end )
{
    size_t cnt = 0;
    while( begin != end ) { ++begin; ++cnt; };
    return cnt;
};


#endif /* _MY_BOOST_UTIL_H_ */
