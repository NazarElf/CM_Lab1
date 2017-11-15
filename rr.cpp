#include "rr.h"

RR::RR(double mu, double lambda)
{
    this->lambda = lambda;
    this->mu = mu;
}

void RR::Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime, double unusualTime, int uniqNum)
{

    fullTaskTime.clear();
    int Index = 0;
    int tasksCompleated = 0;
    int successfulyCompleatedTasks = 0;

    onProc.tLeft = 0;
    onProc.tIn = 0;
    double waitTime = taskWaitTime[Index];
    double tauLeft = tau;

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
        if((tauLeft > waitTime || onProc.tLeft == 0) && Index < max)
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
                //tauLeft -= waitTime;
            }
            else
            {
                onProc = in;
                onProc.started = true;
                reactTime.push_back(0);
            }
            ++Index; waitTime = taskWaitTime[Index];

        } else //if (onProc.tLeft < taskWaitTime[Index])
        {
            //tasksCompleated ++;
            systemTime += tauLeft; //onProc.tLeft;
            waitTime -= tauLeft;
            onProc.tLeft -= tauLeft;

            if (onProc.tLeft <= 0)
            {
                tasksCompleated++;
                successfulyCompleatedTasks++;
                if(!queue.empty())
                {
                    onProc.tIn = systemTime + onProc.tLeft - onProc.tIn;
                    fullTaskTime.push_back(onProc);
                    double temp = onProc.tLeft;
                    onProc = queue.front();
                    if(!onProc.started)
                    {
                        reactTime.push_back(systemTime - onProc.tIn);
                        onProc.started = true;
                    }
                    onProc.tLeft += temp;
                    queue.pop();
                }
                else
                {
                    onProc.tIn = systemTime + onProc.tLeft - onProc.tIn;
                    onProc.tLeft = 0;
                    fullTaskTime.push_back(onProc);
                }
            }
            else if (!queue.empty()){
                queue.push(onProc);
                onProc = queue.front();
                queue.pop();
                if(!onProc.started)
                {
                    onProc.started = true;
                    reactTime.push_back(systemTime - onProc.tIn);
                }
            }
            tauLeft = tau;
        }
    }
    nonLostRate = (double)successfulyCompleatedTasks / (double)tasksCompleated;
    countMx_Dx();
    countUniqMxDx(uniqNum);
}
