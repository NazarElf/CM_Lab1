
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
    for(auto val: this->fullTaskTime)
    {
        mx += val;
    }
    mx /= fullTaskTime.size();
    Dx = 0;
    for(auto val: this->fullTaskTime)
    {
        Dx += (mx - val) * (mx - val);
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

ProcessingUnit::~ProcessingUnit()
{
}

