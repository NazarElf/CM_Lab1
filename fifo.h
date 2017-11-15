#ifndef FIFO_H
#define FIFO_H
#include "processingunit.h"
#include <queue>

class FIFO : public ProcessingUnit
{
public:
    FIFO(double mu, double lambda);
    virtual void Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime, double unusualTime, int uniqNum) override;
    std::queue<task> queue;
};

#endif // FIFO_H
