#include "sf.h"
#include <algorithm>

SF::SF(double mu, double lambda)
{
    this->lambda = lambda;
    this->mu = mu;
}

void SF::Start(const std::vector<double> &taskLength, const std::vector<double> &taskWaitTime)
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
                queue.insert(in);
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
				auto temp = std::min_element(queue.begin(), queue.end());
				onProc = *temp;
				queue.erase(temp);
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

bool operator<(task const & l, task const & r)
{
	return l.tLeft < r.tLeft;
}
