#include <vector>
#include <math.h> 
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <fstream>

/**Solving differential equation of the gravitating point cloud problem
 */

/*vector structure is:

X = [ x1, y1, vx1, vy1,    x2, y2, vx2, vy2, .... ]
*/

typedef std::vector<double> vector_type;

void calc_derivative( const vector_type &X, vector_type &dX );




///////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////
/**Linear conbination of two vectors*/
void lincomb( const vector_type & A, double ka, const vector_type &B, double kb, vector_type &C)
{
    if (C.size() != A.size()){
	C.resize( A.size() );
    }
    vector_type::const_iterator 
	ia=A.begin(),
	ib = B.begin(),
	ia_e = A.end();
    vector_type::iterator 
	ic = C.begin();

    for(; ia != ia_e; ++ia, ++ib, ++ic){
	*ic = ka*(*ia)+kb*(*ic);
    }
}

/** B+= A*k*/
void lincomb_accum( const vector_type &A, double k, vector_type &B)
{
    vector_type::const_iterator
	ia = A.begin(),
	ea = A.end();
    vector_type::iterator 
	ib = B.begin();
    for(; ia!=ea; ++ia, ++ib){
	*ib += (*ia)*k;
    }
}

/*
  F = (x1-x2)/|x1-x2| * |x1-x2|^(-2)
*/
double sqr(double x)
{
    return x*x;
}

struct parameters{
    double eps2;
    double G;
    parameters(){
	eps2 = 1e-5;
	G = 10;
    };
};

void calc_derivative( const vector_type &X, vector_type & dX, const parameters &params)
{
    const double G = params.G;
    const double eps2 = params.eps2;

    if (dX.size() != X.size())
	dX.resize( X.size());

    //relative Indices in the vector array
    const int ix = 0;
    const int iy = 1;
    const int ivx = 2;
    const int ivy = 3;
    
    //Iterate all points...
    for( int idx = 0; idx<X.size(); idx+=4){ //step is 4, because point has 4 coordinates
	//calculate force, applied by other points to this one
	double fx=0, fy=0;
	const double &x=X[idx+ix];
	const double &y=X[idx+iy];

	//to do it, sum up the force, produced by all other points (not really effective, but good precision and algorithm is simple)
	for ( int j = 0; j<X.size(); j += 4){
	    if ( j == idx ) continue; //skippint self...
	    
	    const double &x1=X[ j + ix ];
	    const double &y1=X[ j + iy ];
	    
	    double d2 = sqr(x1-x)+sqr(y1-y);
	    double id3;
	    if (d2 > eps2 ){
		id3 = 1/( sqrt(d2)*d2 );
	    }else{
		id3 = 0;
	    }
	    
	    fx += (x1-x) * id3;
	    fy += (y1-y) * id3;
	}
	//Force calculated.
	//Now calculate derivatives...
	dX[ idx+ix ] = X[ idx+ivx ];
	dX[ idx+iy ] = X[ idx+ivy ];
	dX[ idx+ivx ] = fx*G;
	dX[ idx+ivy ] = fy*G;
    };
};


struct rkutta4_solver{
    vector_type k1, k2, k3, k4;
    vector_type X1, X2, X3;
    parameters params;

    void step( vector_type& X, double dt);
    rkutta4_solver(){
    };
};

struct euler_solver{
    vector_type dX;
    parameters params;
    void step( vector_type& X, double dt);
    euler_solver(){
    };
};

void euler_solver::step( vector_type &X, double dt)
{
    calc_derivative( X, dX, params);
    lincomb_accum( dX, dt, X);
}

void rkutta4_solver::step( vector_type &X, double dt)
{
    calc_derivative( X, k1, params ); //k1 = F( X )

    lincomb( X, 1, k1, dt*0.5, X1);// X1 = X + dt/2 * X'
    calc_derivative( X1, k2, params );//k2 = F( X+ dt/2 k1')

    lincomb( X, 1, k2, dt*0.5, X2); // X2 = X + dt/2 * X1'
    calc_derivative( X2, k3, params); //k3 = F( X+ dt/2 * k2)

    lincomb( X, 1, k3, dt, X3);
    calc_derivative( X3, k4, params); //k4 = F( X + dt * k3)
    
    //X[n+1 ] = X[n] + dt/6*( k1+2*k2+2*k3+k4 )
    lincomb_accum( k1, dt/6, X);
    lincomb_accum( k2, dt/3, X);
    lincomb_accum( k3, dt/3, X);
    lincomb_accum( k4, dt/6, X);
}


/////////////////////////////////
//Initializers
/////////////////////////////////

double random_d()
{
    return double(rand())/double(INT_MAX);
};

void rand_point_in_disk( double r, double& x, double &y)
{
    double r2 = r*r;
    do{
	x = (random_d()*2-1)*r;
	y = (random_d()*2-1)*r;
    }while ( sqr(x)+sqr(y)>r2);
}
void rand_speed_vector( double vmax, double& vx, double &vy)
{
    //make random direction
    double alpha = random_d()*2*M_PI;
    double v = random_d()*vmax;
    vx = cos(alpha)*v;
    vy = sin(alpha)*v;
}
    
/**init disc of points*/
void init_x( vector_type &X, double r, double v )
{
    for(int i =0; i<X.size(); i +=4){
	rand_point_in_disk( r, X[i], X[i+1] );
	rand_speed_vector( v, X[i+2], X[i+3] );
    }
}

/////////////////////////////////////
// Saving data 
/////////////////////////////////////
void save_gnuplot( const vector_type &X, std::ostream & strm )
{
    for( int i =0; i<X.size(); i += 4){
	strm<<X[i+0]<<"\t"<<X[i+1]<<std::endl;
    }
}
void save_gnuplot_xv( const vector_type &X, std::ostream & strm )
{
    for( int i =0; i<X.size(); i += 4){
	strm<<X[i+0]<<"\t"<<X[i+1]<<"\t";
	strm<<X[i+2]<<"\t"<<X[i+3]<<std::endl;
    }
}

//////////////////////////////
// Test
//////////////////////////////

int main(int argc, char *argv[])
{
    int num_points = 100;
    double dt = 0.01;
    double init_r = 20;
    double init_v = 1;

    double T = 100;

    vector_type X;
    X.resize( num_points*4 );
    init_x( X, init_r, init_v );

    X[0] = -1; X[1] = 0;
    X[2] =  0; X[3] = 1;

    X[4] =  1; X[5] = 0;
    X[6] =  0; X[7] = -1;


    std::cout<<"Starting solution with "<<num_points<<" points..."<<std::endl;

    rkutta4_solver solver;
//    euler_solver solver;

    solver.params.G = 5;

    double t = 0;
    int last_progress = 0;
    while (t<T){
	solver.step( X, dt );
	t += dt;
	int progress = 100*t/T;
	if (progress-last_progress > 5){
	    std::cout<<"Progress: "<<progress<<"%   \r";
	    std::cout.flush();
	    last_progress = progress;
	}
    };
    std::cout<<"\n";

    std::cout<<"Solved"<<std::endl;

    std::ofstream ofile( "cloud.dat" );
    save_gnuplot_xv( X, ofile );

    std::cout<<"Saved data cloud.dat"<<std::endl;
    return 0;
}

