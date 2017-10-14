#ifndef RR_H
#define RR_H
#include "processingunit.h"
#include <queue>

class RR : public ProcessingUnit
{
public:
    double tau = 0.001;
    RR(double mu, double lambda);
    void Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime) override;
    std::queue<task> queue;
};

#endif // RR_H
