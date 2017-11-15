#ifndef SF_H
#define SF_H
#include "processingunit.h"
#include <set>

bool operator<(task const& l, task const& r);

class SF : public ProcessingUnit
{
public:
    SF(double mu, double lambda);
    void Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime, double unusualTime, int uniqNum) override;
	std::multiset<task> queue;
};


#endif // SF_H
