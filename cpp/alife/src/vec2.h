/***************************************************************************
 *            vec2.h
 *
 *  Sun Jan 31 00:45:29 2010
 *  Copyright  2010  dim
 *  <dim@<host>>
 ****************************************************************************/

#ifndef VEC2_INCLUDED
#define VEC2_INCLUDED

#include <math.h>

typedef float ftype;


template <typename T>
T sqr( T x )
{
	return x*x;
};

template <typename T>
T min( T x, T y)
{
	if (x>y) return y;
	else     return x;
}
template <typename T>
T max( T x, T y)
{
	if (x<y) return y;
	else     return x;
}

template <typename T>
T limit( T x, T min_value, T max_value)
{
	return min(max(x, min_value), max_value);
}


class vec2
{
	public:
		ftype x,y;

		vec2( ftype nx, ftype ny): x(nx), y(ny) {};
		vec2() {};

		vec2& operator +=( const vec2& v){
			x+= v.x;
			y+= v.y;
			return *this;
		};
		vec2& operator -=( const vec2& v){
			x-= v.x;
			y-= v.y;
			return *this;
		};
		vec2 operator +( const vec2& v)const{
			return vec2( x+v.x, y+v.y);
		};
		vec2 operator -( const vec2& v)const{
			return vec2( x-v.x, y-v.y);
		};

		vec2& operator *=( ftype k){
			x*=k;
			y*=k;
			return *this;
		};
		vec2 operator *( ftype k)const{
			return vec2(x*k, y*k);
		};
		ftype norm2() const{
			return sqr(x)+sqr(y);
		};
		ftype norm() const{
			return (ftype)sqrt( norm2() );
		};
		vec2& normalize(){
			return (*this) *= (1/norm());
		};
};

class rotation{
	private:
		rotation( ftype c, ftype s): v(c, s){};
		
	public:
		vec2 v; //con_sin vector

		explicit rotation( ftype alpha ): v( cos(alpha), sin( alpha ) ) {};
		
		rotation(){};

		ftype angle()const{
			return (ftype)atan2(v.x, v.y);
		}

		void normalize(){
			v.normalize();
		}

		void set( ftype alpha ){
			v = vec2( cos(alpha), sin(alpha));
		}

		void setSmall( ftype alpha){
			v = vec2( 1-sqr(alpha)*(ftype)0.5, alpha);
		}

		rotation operator +( const rotation &r)const{
			return rotation( v.x * r.v.x - v.y * r.v.y, v.x * r.v.y + v.y * r.v.x );
		}
		
		rotation operator -( const rotation &r)const{
			return rotation( v.x*r.v.x+v.y*r.v.y, -v.x*r.v.y+v.y*r.v.x );
		}

		vec2 apply( const vec2 &vec)const{
			return vec2( vec.x*v.x - vec.y*v.y, vec.x*v.y + vec.y*v.x);
		}
};
//Some helper geometry fucntions
inline
bool in_rect(const vec2& v, ftype x0, ftype y0, ftype x1, ftype y1)
{
	return v.x >= x0 && v.x<=x1 && v.y>=y0 && v.y<=y1;
};
inline
bool in_rect(const vec2& v, const vec2& p0, const vec2& p1)
{
	return in_rect(v, p0.x, p0.y, p1.x, p1.y);
};
inline
ftype dist2(const vec2& v1, const vec2& v2)
{
	return (v1-v2).norm2();
}
inline
ftype dist(const vec2& v1, const vec2& v2)
{
	return (v1-v2).norm();
}

/**Pseudo-scalar vector product*/
inline
ftype psrod(const vec2& v1, const vec2& v2)
{
   return v1.x*v2.y - v1.y*v2.x;
}
/**scalar product*/
inline ftype sprod( const vec2& v1, const vec2& v2)
{
   return v1.x*v2.x+v1.y*v2.y;
}
#endif
