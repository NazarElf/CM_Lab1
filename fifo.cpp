#include "fifo.h"
#include <random>

FIFO::FIFO(double mu, double lambda)
{
    this->lambda = lambda;
    this->mu = mu;
}

void FIFO::Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime, double unusualTime, int uniqNum)
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
                onProc = queue.front();
                reactTime.push_back(systemTime - onProc.tIn);
                queue.pop();
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
                queue.push(in);
                onProc.tLeft -= waitTime;
            }
            else
            {
                onProc = in;
                reactTime.push_back(0);
            }
            ++Index; waitTime = taskWaitTime[Index];

        } else if (onProc.tLeft != 0)
        {
            tasksCompleated ++;
            successfulyCompleatedTasks++;
            systemTime += onProc.tLeft;
            waitTime -= onProc.tLeft;
            if(!queue.empty())
            {
                onProc.tIn = systemTime - onProc.tIn;
                fullTaskTime.push_back(onProc);
                onProc = queue.front();
                reactTime.push_back(systemTime - onProc.tIn);
                queue.pop();
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
