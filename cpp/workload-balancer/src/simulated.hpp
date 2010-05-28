#ifndef _SIMULATED_H_
#define _SIMULATED_H_
class Simulated{
public:
    virtual bool simulate()=0; //perform one simulation step. Returns true, if simulation must be continued, false, if simulation finished.
};

#endif /* _SIMULATED_H_ */
