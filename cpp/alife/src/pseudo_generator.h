#pragma once
#ifndef _PSEUDO_GENERATOR_
#define _PSEUDO_GENERATOR_
#include <stdexcept>

////configuration
/**Define it to enable checks of the generator state. This causes a bit slower code, but 
 instead of producing incorrect results it would throw std::logic_error.
 Affects:
 	generator_iterator::operator ++
 	generator_iterator::operator *
 */
#define SAFE_PSEUDO_GENERATOR



/* This file is a header library, simplifying creation of the pseudo-generators,
 i.e., it imitates continuations. 
*/

/**Default initialization for the basic types*/
template< typename T, T default_value>
class default_initialized{
	public:
		T value;
		default_initialized(): value( default_value){};
		default_initialized( const T& val ): value( val ){};
		default_initialized& operator=( const T& val){
			value=val;
			return *this;
		};
		bool operator ==( const T& val)const{ return value == val;};
		bool operator !=( const T& val)const{ return value != val;};
		operator const T& () const {return value;};
		operator T& () {return value;};
};


/**Base class for all pseudo_generators.*/
template<typename T>
struct pseudo_generator
{
		typedef T value_type;
};

/* Macros, for simplified declaration of the generator classes
 */
#define BEGIN_GENERATOR_STATES \
	typedef enum{\
		_initial_state_, _end_state_,

#define END_GENERATOR_STATES \
	} _generator_state_type;\
	default_initialized<_generator_state_type, _initial_state_> _continue_;\
	bool stopped()const { return _continue_ == _end_state_; };\
	bool operator()(value_type &v){ \
		v = (*this)();              \
		return !stopped();          \
	}
/*the latter macro allows the following simple form of iteration: 
  for( value_type x; gen( x ); ){
	  syd::cout<<x;
  }
*/

/* These macros MUST be present only once in the generator classs declaration.
  Between them, list of the state labels, comma-separated must be present

 Sample, declaring generator with 3 states.

////////////////////////////////////
class test_generator{
public:
	BEGIN_GENERATOR_STATES
		pos1, pos2, pos3
	END_GENERATOR_STATES;
////////////////////////////////////	

	In parctice, state names are values of the enumeration.
*/
	

/* Implementation macros
 */

// Begins state restoration section. MUST be present at the beginning of the generator body, only once.
// THis macro may be followed by semicolon
#define BEGIN_RESTORE_STATE \
	switch( _continue_.value ){ 
		
// Restore particular code. State name must be one of the names, declared in the class.
// This macro MUST follow the BEGIN_RESTORE_STATE, and MUST be repeated for anu state used.
#define RESTORE_STATE(state_name) \
	case state_name: goto _label_##state_name;
		
//Closes BEGIN_RESTORE_STATE. 
#define END_RESTORE_STATE \
	case _initial_state_: goto _label__initial_state_;\
	case _end_state_: throw std::logic_error("iterate after end");\
	default: throw std::logic_error("Failed to restore state: state is unknown.");\
    }

/* Sample usage:
test_generator::value_type test_generator::value()
{
	BEGIN_RESTORE_STATE;
	RESTORE_STATE( pos1 );
	RESTORE_STATE( pos2 );
	END_RESTORE_STATE;
*/

/*Returns value and stores current state. "state" must be one of the states, declared in the header.
 */
#define YIELD(value, state) \
	_continue_ = state; \
	return (value);\
	_label_##state:;

/*Declare beginning of the generator body. MUST follow the END_RESTORE_STATE.
 */
#define BEGIN_GENERATOR \
	_label__initial_state_:;

/*Closes the body*/
#define END_GENERATOR \
	_continue_ = _end_state_;

/****************************************************
 Helper templates
 */

template<typename generator>
class generator_iterator: 
    public std::iterator<std::input_iterator_tag, typename generator::value_type>{
private:
    typedef enum{
	    status_has_value,//generator called, and some actual value is received.
		status_stopped//generarator finished, value is not actual.
	} iterator_state_type;
    
    iterator_state_type status;
    generator g;
		
    typename generator::value_type 
	cur_value; //value_type is declared in the iterator class
		
	/*Read first value to the buffer and update status*/
	void initialize(){
		//assert( status == status_not_init)
		cur_value = g();
		status = g.stopped() ? status_stopped : status_has_value;
	}
	
public:
#ifndef SAFE_PSEUDO_GENERATOR
	//version without out-of range checks
	typename generator::value_type & operator*(){ 
		return cur_value;
	};
#else
//version with state checks
	typename generator::value_type & operator*(){ 
		switch (status){
			case status_has_value:
				return cur_value;
			case status_stopped:
				throw std::logic_error("Attempt to get value of stopped iterator");
			default:
				throw std::logic_error("Unexpected state. Probably, memory corruption");
		}
	};
#endif
	
	const typename generator::value_type & operator*()const{ 
		return *const_cast<generator_iterator&>(*this);
	};

	
#ifndef SAFE_PSEUDO_GENERATOR
	generator_iterator& operator++(){
		cur_value = g();
		if (g.stopped()) {
			status = status_stopped;
		}
		return *this;
	};
#else
	generator_iterator& operator++(){
		switch (status){
			case status_has_value:
				cur_value = g();
				if (g.stopped())
					status = status_stopped;
				break;
			case status_stopped:
				throw std::logic_error("Attempt to increase after-the-end iterator");
			default:
				throw std::logic_error("Unexpected state when increased iterator. Probably, memory corruption");
		}
		return *this;
	};
#endif
		
	//only end iterators are equal. It is done for the simplicity.
	bool operator == (const generator_iterator & i)const{
		return (status==status_stopped) && (i.status == status_stopped);
	};
	
	bool operator != (const generator_iterator & i)const{
		return !((*this) == i);
	};


	/**Constructors*/

	//default constructor is end constructor
	generator_iterator(): status(status_stopped){ };

        //copy-constructor
        generator_iterator( const generator_iterator & gi)
	:  status (gi.status), g( gi.g ),cur_value( gi.cur_value )
	{
	}

	//construct from existing generator
	generator_iterator(const generator& _g)
		:g(_g)
	{
		initialize();
	}

//Note: passing by value can cause serious problems.
//when trying to pass value by reference (because adapter would pass by value)
//Either use passing by pointer,
//or do not use them at all.

	//using generator own constructors
	template <typename A1>
	generator_iterator(A1 &a)
		:g(a)
	{ 
		initialize(); 
	};

	template <typename A1, typename A2>
	generator_iterator(A1 &a1, A2 &a2)
		:g(a1, a2)
	{			 
		initialize(); 
	};

	template <typename A1, typename A2, typename A3>
	generator_iterator(A1 &a1, A2 &a2, A3 &a3)
		:g(a1, a2, a3)
	{			 
		initialize(); 
	};

	template <typename A1, typename A2, typename A3, typename A4>
	generator_iterator(A1 &a1, A2 &a2, A3 &a3, A4 &a4)
		:g(a1, a2, a3, a4)
	{			 
		initialize(); 
	};

	template <typename A1, typename A2, typename A3, typename A4, typename A5>
            generator_iterator(A1 &a1, A2 &a2, A3 &a3, A4 &a4, A5 &a5)
            :g(a1, a2, a3, a4, a5)
            {			 
		initialize(); 
            };
};
        
///////////////////////////////////////////
//Utility cuntions
template<class generator, class unary_func>
    typename unary_func::result_type map_sum( generator& gen, const unary_func & f, typename unary_func::result_type init = 0)
{
    typename unary_func::result_type val( init );
    for( typename generator::value_type x; gen( x ); ){//iterate over generated values...
        val += f( x );
    }
    return val;
}

/**Copy values from the generator to the inut iterator*/
 template<class generator, class output_iter>
     output_iter gen_copy(generator & g, output_iter itr)
{
    for(typename generator::value_type x; g( x ); ++itr){
        (*itr) = x;
    }
    return itr;
};

/**Copy values from the generator to the inut iterator, not more than specified*/
template<class generator, class output_iter>
output_iter gen_copy(generator & g, output_iter itr, output_iter itr_end)
{
  for(typename generator::value_type x; g( x ) && itr != itr_end; ++itr){
    (*itr) = x;
  }
  return itr;
};





#endif
