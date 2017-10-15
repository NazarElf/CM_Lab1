#ifndef PROCESSINGUNIT_H
#define PROCESSINGUNIT_H
#include <vector>
#include <random>
struct task
{
    double tLeft;
    double tIn;
};

class ProcessingUnit
{
public:
    double tau = 0.001;
    virtual void Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime) = 0;
    void setMu(double mu);
    double getMu();
    void setLambda(double lambda);
    double getLambda();
    double getMx();
    double getDx();
protected:
    double systemTime = 0;
    double mu;
    double lambda;
    task onProc;
    std::vector<double> fullTaskTime;
    double mx;
    double Dx;
    void countMx_Dx();
    ~ProcessingUnit();
};

#endif // PROCESSINGUNIT_H
