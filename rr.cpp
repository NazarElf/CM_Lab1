#include "rr.h"

RR::RR(double mu, double lambda)
{
    this->lambda = lambda;
    this->mu = mu;
}

void RR::Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime)
{

    fullTaskTime.clear();
    int Index = 0;
    int tasksCompleated = 0;

    onProc.tLeft = 0;
    onProc.tIn = 0;
    double waitTime = taskWaitTime[Index];
    double tauLeft = tau;

    int max = taskLength.size();
    while (tasksCompleated < max)
    {
        if((tauLeft > waitTime || onProc.tLeft == 0) && Index < max)
        {
            task in;
            systemTime += waitTime;
            in.tIn = systemTime;
            in.tLeft = taskLength[Index];
            if(onProc.tLeft != 0)
            {
                queue.push(in);
                tauLeft -= waitTime;
            }
            else
                onProc = in;
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
                if(!queue.empty())
                {
                    onProc.tIn = systemTime +onProc.tLeft - onProc.tIn;
                    fullTaskTime.push_back(onProc.tIn);
                    onProc = queue.front();
                    queue.pop();
                }
                else
                {
                    onProc.tIn = systemTime + onProc.tLeft - onProc.tIn;
                    onProc.tLeft = 0;
                    fullTaskTime.push_back(onProc.tIn);
                }
            }
            else if (!queue.empty()){
                queue.push(onProc);
                onProc = queue.front();
                queue.pop();
            }
            tauLeft = tau;
        }
    }
    countMx_Dx();

}
