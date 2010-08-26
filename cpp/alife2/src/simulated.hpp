#pragma once
#ifndef _SIMULATED_H_
#define _SIMULATED_H_
namespace alife2{
    /**Objects, implementing this interface support step-by-step discrete time simulation*/
    class Simulated{
    public:
	virtual bool simulate() = 0;
    };
};


#endif /* _SIMULATED_H_ */
