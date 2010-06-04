#ifndef _BALANCE_METHOD_H_
#define _BALANCE_METHOD_H_

#include "balancer_types.hpp"

/**Virtual interface for the balancing algorithm
 */
class BalanceMethod{
protected:
    SizesVector N2; //calculated value, returned by the valvulate(...) method
public:
    //First call
    virtual void first( const SizesVector & N0, const TimesVector & T0) = 0;

    //called to calculate balance
    virtual SizesVector & calculate( const SizesVector &N0, const TimesVector T0, 
				     const SizesVector &N1, const TimesVector T1 ) = 0;
    virtual ~BalanceMethod(){};
};

class BM_Simple: public BalanceMethod{
private:
    //Values of Ni and Ti on this sten and on previous step.
    std::vector< double > throughput;
    std::vector< double > exactSizes;
    TimeType catchTimeInterval;

public:
    BM_Simple( TimeType catchTime );
    //First call
    virtual void first( const SizesVector & N0, const TimesVector & T0);

    //called to calculate balance
    virtual SizesVector & calculate( const SizesVector &N0, const TimesVector T0, 
				     const SizesVector &N1, const TimesVector T1 );
    virtual ~BM_Simple(){};

private:
    void updateSize( size_t N );
    
};

#endif /* _BALANCE_METHOD_H_ */
