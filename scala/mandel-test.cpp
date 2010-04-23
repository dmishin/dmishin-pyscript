#include <iostream>
#include <time.h>

int calcNoCplx( double z_re, double z_im)
{
    double x=0,y=0;
    int iter = 0;

    do{
	double x2 = x*x;
	double y2 = y*y;
	if (x2+y2 > 4) return iter;

	double xx = x2-y2+z_re;
	y = 2*x*y+z_im;
	x = xx;

	iter += 1;
	if (iter > 2000) return -1;
    }while( true );
    return 0;
}

void calcRectNoCplx( double x0,double  y0, double x1, double y1, int w, int h) 
{
    int *out = new int[ w*h ];

    double kx = (x1-x0) / (double) w;
    double ky = (y1-y0) / (double)h ;

    for( int iy = 0; iy < h; ++ iy){
	for( int ix = 0; ix<w; ++ix) {
	    int idx = iy * w + ix ;
	    out[ idx ] = calcNoCplx( ix * kx + x0, iy * ky + y0);
	}
    }
}
 
int main(int argc, char *argv[])
{
    const time_t timer = time( NULL );
    calcRectNoCplx( -2, -2, 2, 2, 2000, 2000);
    const time_t timer1 = time( NULL );
    double elapsed = difftime( timer1, timer );
    std::cout<<"Elapsed time (ms):"<<elapsed*1000<<std::endl;
    return 0;
}

