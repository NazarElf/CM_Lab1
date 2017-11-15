#include "sf.h"
#include <algorithm>

SF::SF(double mu, double lambda)
{
    this->lambda = lambda;
    this->mu = mu;
}

void SF::Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime, double unusualTime, int uniqNum)
{
    fullTaskTime.clear();
    int Index = 0;
    int tasksCompleated = 0;
    int successfulyCompleatedTasks = 0;

    onProc.tLeft = 0;
    onProc.tIn = 0;
    double waitTime = taskWaitTime[Index];

    int max = taskLength.size();
    while (tasksCompleated < max)
    {
        if((4 < systemTime - onProc.tIn + onProc.tLeft || onProc.tLeft > 4)&& onProc.tLeft !=0)
        {
            onProc.tLeft = 0;
            tasksCompleated++;
            if(!queue.empty())
            {
                onProc.tIn = systemTime - onProc.tIn;
                fullTaskTime.push_back(onProc);
                auto temp = std::min_element(queue.begin(), queue.end());
                onProc = *temp;
                queue.erase(temp);
                reactTime.push_back(systemTime - onProc.tIn);
            }
            continue;
        }
        if((onProc.tLeft > waitTime || onProc.tLeft == 0) && Index < max)
        {
            task in;
            systemTime += waitTime;
            in.tIn = systemTime;
            in.tLeft = taskLength[Index];
            if(in.tLeft == unusualTime )
            {
                in.time = true;
            }
            if(onProc.tLeft != 0)
            {
                queue.insert(in);
                onProc.tLeft -= waitTime;
            }
            else
            {
                onProc = in;
                reactTime.push_back(0);
            }
            ++Index; waitTime = taskWaitTime[Index];

        } else //if (onProc.tLeft < taskWaitTime[Index])
        {
            tasksCompleated ++;
            successfulyCompleatedTasks++;
            systemTime += onProc.tLeft;
            waitTime -= onProc.tLeft;
            if(!queue.empty())
            {
                onProc.tIn = systemTime - onProc.tIn;
                fullTaskTime.push_back(onProc);
				auto temp = std::min_element(queue.begin(), queue.end());
				onProc = *temp;
				queue.erase(temp);
                reactTime.push_back(systemTime - onProc.tIn);
            }
            else
            {
                onProc.tLeft = 0;
                onProc.tIn = systemTime - onProc.tIn;
                fullTaskTime.push_back(onProc);
            }
        }
    }
    nonLostRate = (double)successfulyCompleatedTasks / (double)tasksCompleated;
    countMx_Dx();
    countUniqMxDx(uniqNum);
}

bool operator<(task const & l, task const & r)
{
	return l.tLeft < r.tLeft;
}
