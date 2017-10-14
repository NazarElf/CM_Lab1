#include "fifo.h"
#include <random>

FIFO::FIFO(double mu, double lambda)
{
    this->lambda = lambda;
    this->mu = mu;
}

void FIFO::Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime)
{
    fullTaskTime.clear();
    int Index = 0;
    int tasksCompleated = 0;

    onProc.tLeft = 0;
    onProc.tIn = 0;
    double waitTime = taskWaitTime[Index];

    int max = taskLength.size();
    while (tasksCompleated < max)
    {
        if((onProc.tLeft > waitTime || onProc.tLeft == 0) && Index < max)
        {
            task in;
            systemTime += waitTime;
            in.tIn = systemTime;
            in.tLeft = taskLength[Index];
            if(onProc.tLeft != 0)
            {
                queue.push(in);
                onProc.tLeft -= waitTime;
            }
            else
                onProc = in;
            ++Index; waitTime = taskWaitTime[Index];

        } else //if (onProc.tLeft < taskWaitTime[Index])
        {
            tasksCompleated ++;
            systemTime += onProc.tLeft;
            waitTime -= onProc.tLeft;
            if(!queue.empty())
            {
                onProc.tIn = systemTime - onProc.tIn;
                fullTaskTime.push_back(onProc.tIn);
                onProc = queue.front();
                queue.pop();
            }
            else
            {
                onProc.tLeft = 0;
                onProc.tIn = systemTime - onProc.tIn;
                fullTaskTime.push_back(onProc.tIn);
            }
        }
    }
    countMx_Dx();
}
