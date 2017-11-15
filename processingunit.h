#ifndef PROCESSINGUNIT_H
#define PROCESSINGUNIT_H
#include <vector>
#include <random>
struct task
{
    double tLeft;
    double tIn;
    bool time = false;
    bool started = false;
};

class ProcessingUnit
{
public:
    double nonLostRate = 1.0;
    double tau = 0.001;
    virtual void Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime, double unusualTime, int uniqNum) = 0;
    void setMu(double mu);
    double getMu();
    void setLambda(double lambda);
    double getLambda();
    double getMx();
    double getDx();
    double getUniqMx();
    double getUniqDx();
    double countReactTime();
protected:
    double systemTime = 0;
    double mu;
    double lambda;
    task onProc;
    std::vector<task> fullTaskTime;
    std::vector<double> reactTime;
    double mx;
    double Dx;
    double uniqMx;
    double uniqDx;

    void countMx_Dx();
    void countUniqMxDx(int uniqNum);
    ~ProcessingUnit();
};

#endif // PROCESSINGUNIT_H
