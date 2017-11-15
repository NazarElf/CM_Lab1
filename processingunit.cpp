
#include "processingunit.h"

void ProcessingUnit::setMu(double mu)
{
    this->mu = mu;
}
double ProcessingUnit::getMu()
{
    return this->mu;
}
void ProcessingUnit::setLambda(double lambda)
{
    this->lambda = lambda;
}
double ProcessingUnit::getLambda()
{
    return this->lambda;
}
void ProcessingUnit::countMx_Dx()
{
    mx = 0;
    for(task val: this->fullTaskTime)
    {
        mx += val.tIn;
    }
    mx /= fullTaskTime.size();
    Dx = 0;
    for(task val: this->fullTaskTime)
    {
        Dx += (mx - val.tIn) * (mx - val.tIn);
    }
    Dx /= fullTaskTime.size();
}
double ProcessingUnit::getMx()
{
    return mx;
}
double ProcessingUnit::getDx()
{
    return Dx;
}

void ProcessingUnit::countUniqMxDx(int uniqNum)
{
    uniqMx = 0;
    for(task val: this->fullTaskTime)
    {
        if(val.time)
            uniqMx += val.tIn;
    }
    uniqMx /= (fullTaskTime.size() / uniqNum);
    uniqDx = 0;
    for(task val: this->fullTaskTime)
    {
        if(val.time)
            uniqDx += (uniqMx - val.tIn) * (uniqMx - val.tIn);
    }
    uniqDx /= (fullTaskTime.size() / uniqNum);
}

double ProcessingUnit::getUniqMx()
{
    return uniqMx;
}
double ProcessingUnit::getUniqDx()
{
    return uniqDx;
}

double ProcessingUnit::countReactTime()
{
    double a = 0;
    for(auto val: this->reactTime)
    {
        a += val;
    }
    return a/=reactTime.size();
}

ProcessingUnit::~ProcessingUnit()
{
}

