#include<bits/stdc++.h>
using namespace std;
#define endl '\n'
#define int long long

struct proc{
    int jobId;
    int jobSize;
    int priority;
    int arrivalTime;

    proc(int jobId, int jobSize, int priority)
        : jobId(jobId),jobSize(jobSize),priority(priority)
    {
        //jobId = jobid;
    }
};


bool ComparePriority(proc const& p1, proc const& p2)
{
    return p1.priority > p2.priority;
}

signed main(){
    queue<proc> pq1,pq2,pq3;
    vector<proc> rq;
    int rq_size; 
    cout<< "Please enter the size of Ready Queue:";
    cin >> rq_size;  
    int id=1;
    int servedJobs=0;
    for(int i=0;i<300;i++,id++)
    {
        int priority = rand()%3+1;
        int job_size = rand()%4+1;
        proc job(id,priority,job_size);
        if(priority==1) pq1.push(job);
        else if(priority==2) pq2.push(job);
        else if(priority==3) pq3.push(job);

        while(!pq3.empty() and rq.size()<rq_size){
            pq3.front().arrivalTime = i;
            rq.push_back(pq3.front());
            pq3.pop();
        }
        while(!pq2.empty() and rq.size()<rq_size){
            pq2.front().arrivalTime = i;
            rq.push_back(pq2.front());
            pq2.pop();
        }
        while(!pq1.empty() and rq.size()<rq_size){
            pq1.front().arrivalTime = i;
            rq.push_back(pq1.front());
            pq1.pop();
        }

        sort(rq.begin(),rq.end(),ComparePriority);
        rq[0].jobSize--;
        if(rq[0].jobSize==0){
            rq.erase(rq.begin());
            servedJobs++;
        }
        for(int j=0;j<rq.size();j++){
            if((i-rq[j].arrivalTime)%10==9)
                rq[j].priority++;
        }

    }
    cout << "Total number of jobs generated: " << id-1 << endl;
    cout << "Total jobs served by CPU: " << servedJobs << endl;
    cout << "Number of jobs remaining in ready queue:" << rq.size() << endl;
    cout << "Number of jobs lost:" << pq1.size()+pq2.size()+pq3.size() << endl;
}