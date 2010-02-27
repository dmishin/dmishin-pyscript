#pragma once
#ifndef __BODY_H_INCLUDED__
#define __BODY_H_INCLUDED__
/**abstract interfaceforthe "body": aggregate of sensors and motors*/
class Body
{
public:
    virtual int getNumMotors()const = 0;
    virtual int getNumSensors()const = 0;
    virtual void setMotor( int idx, ftype value)=0;
    virtual ftype getSensor( int idx)const = 0;
};

#endif
