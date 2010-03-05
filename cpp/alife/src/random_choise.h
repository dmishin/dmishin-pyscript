#pragma once
#ifndef __RANDOM_CHOISE__
#define __RANDOM_CHOISE__
#include <vector>
class RandomChoise{
private:
    typedef std::pair<double, int> Choise;
    typedef std::vector<Choise> Choises;
    Choises choises;
    double sumP;
public:
    RandomChoise() :sumP(0){};
    void add(double probability, int value);
    int get()const;
    int numChoises()const{return choises.size();};
    bool empty()const{ return numChoises() == 0;};
private:
    int search(double v, int begin, int end)const;
};
#endif
