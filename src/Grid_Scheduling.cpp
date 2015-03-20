//
//  Grid_Scheduling.cpp
//  1. Cost Optimization
//  2. Time Optimization
//  3. Conservative Time Optimization
//
//  Created by Prabhdeep Singh Walia on 15/02/15.
//  Copyright (c) 2015 Prabhdeep Singh Walia. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>
#include <map>
#include <vector>
#include <math.h>
#include <algorithm>
#include <list>
#include <fstream>
using namespace std;

#define REP(i,n) for (int i = 0; i < n; i++)
#define in(i) scanf("%d",&i)
#define out(i) printf("%d\n",i)

typedef long long ll;
const ll MOD = 1000000007;
const int maxn=1e+5;
typedef vector<vector<ll> > matrix;
//define matrix(KxK) as : matrix C(K+1, vector<ll>(K+1));
//map<int,int> m;
/*Check Time:
 clock_t startTime=clock()
 cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds.";
 */

 ll M(ll x,ll y)
 {
 return (x*y)%MOD;
 }
 ll A(ll x,ll y)
 {
 return (x+y)%MOD;
 }
 ll mod_pow(ll b,ll e)
 {
 if(e==0)    {return 1;}
 else if(e%2==0) {return mod_pow(M(b,b),e/2);}
 else    {return M(b,mod_pow(b,e-1));}
 }
 ll inv(ll x)
 {
 return mod_pow(x,MOD-2);    //using fermat's little theorem x^(p-1)=1 mod p (p is prime)
 }
 ll D(ll x,ll y)
 {
 return M(x,inv(y));
 }
 

int main(int argc, const char * argv[])
{
    int deadline[]={990, 1980, 2970};
    int budget[] = {126000, 171000, 252000};
    int Qcosts[] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 28};
    int jobs = 100;
    int cost = 0;
    vector<int> assigned[10];
    int jobtime[100];
    int nct[10];
    int totassign[10];
    
    cout<<"Cost Optimization Scheduling:\n\n";
    /* Step 1 : sort resources by cost*/
    sort(Qcosts, Qcosts+10);
    jobs = 100;
    REP(i, 3)  //deadline
    {
        REP(j, 3)   //budget
        {
            jobs=100;
            int x = deadline[i]/100; //max jobs a queue can perform in allotted time
            int k=0;
            cost = 0;
            int add=0;
            while(jobs>0)   //allot as many jobs to cheaper resource and then move to next resource
            {
                if(jobs>x)
                {    add=(x*90*Qcosts[k]);
                    if(add<0)   cout<<x<<" "<<k<<endl;
                }
                else
                {   add=(jobs*90*Qcosts[k]);
                    if(add<0)   cout<<x<<" "<<k<<endl;
                }
                if(cost+add<budget[j])  //allot the job if budget criteria satisfies
                {
                    cost+=add;
                    if(jobs>x)
                        jobs-=x;
                    else
                        jobs = 0;
                    k++;
                    if(k>=10)   break;
                }
                else
                    break;
                if(k>=10)   break;
            }
            add = 90*Qcosts[k]; //allot as many jobs as possible to the latest costly queue
            while(jobs>0)
            {
                if(cost+add<=budget[j])
                {
                    jobs--;
                    cost+=add;
                }
                else
                    break;
            }
            
            cout<<deadline[i]<<"\t"<<budget[j]<<"\t"<<cost<<"\t"<<endl;
            //cout<<100-jobs<<endl;
        }
    }
 
    cout<<"\nTime Optimization Schedule:\n";
    
    memset(jobtime, 90, sizeof(jobtime));   //each job takes 90s
    REP(i, 3)   //deadline
    {
        REP(j, 3)   //budget
        {
            memset(nct, 0, sizeof(nct));
            memset(totassign, 0, sizeof(totassign));
            REP(p, 10)  assigned[p].erase(assigned[p].begin(), assigned[p].end());
            jobs = 100;
            cost = 0;
            int notdone = 0;
            int k=1;
            for(; k<=jobs; ++k)  //jobs
            {
                REP(r, 10)  //calculate next completion time for each resource
                {
                    nct[r] = 90;
                    for(auto x : assigned[r])
                    {
                        nct[r]+=jobtime[x];
                    }
                }
                
                /*find resource with min nct and such 
                 that cost per job is less than
                 remaining budget per remaining jobs*/
                int min = INT32_MAX;
                int ind = -1;
                REP(r, 10)
                {
                    int cpj = 90*Qcosts[r];
                    if(cpj<=(budget[j]-cost)/(jobs-k+1))
                    {
                        if(nct[r]<min && assigned[r].size()<10)
                        {
                            min = nct[r];
                            ind = r;
                        }
                    }
                }
                
                /*the following step checks for violation of deadline*/
                int max = -1;
                REP(r, 10)
                {
                    if(totassign[r]>max)
                        max = totassign[r];
                }
                if(max*90 > (deadline[i]*90)/100)    break;
                
                /*allot the job if budget is not exceeded*/
                if(ind>=0)
                {
                    cost+=90*Qcosts[ind];
                    assigned[ind].push_back(k);
                    totassign[ind]++;
                }
                else    notdone++;
            }
            cout<<deadline[i]<<"\t"<<budget[j]<<"\t"<<cost<<"\t"<<endl;
            //cout<<k-notdone-1<<endl;
        }
    }
  
  
    cout<<"\nConservative Time Optimization Schedule:\n";
    vector<int> usethis;
    REP(i, 3)
    {
        REP(j, 3)
        {
            memset(nct, 0, sizeof(nct));
            memset(totassign, 0, sizeof(totassign));
            REP(p, 10)  assigned[p].erase(assigned[p].begin(), assigned[p].end());
            jobs = 100;
            cost = 0;
            int k=1;
            for(; k<=jobs;)  //jobs
            {
                usethis.erase(usethis.begin(), usethis.end());
                /*calculate next completion time for each resource*/
                REP(r, 10)
                {
                    nct[r] = 90;
                    nct[r]+=(90*assigned[r].size());
                }
                /*check which resources satisfies the condition 
                 that cost per job is less than
                 remaining budget per remaining jobs*/
                REP(r, 10)
                {
                    int cpj = 90*Qcosts[r];
                    if(cpj<=(budget[j]-cost)/(jobs-k+1))
                    {
                        usethis.push_back(r);
                    }
                }
                
                /*find which of the allowed resource has highest nct*/
                int max = 0;
                for(auto x : usethis)
                {
                    if(nct[x]>max)  max = nct[x];
                }
                
                /*allot jobs according to inverse proportion of nct of allowed resources*/
                bool flag = false;
                int t=0;
                for(auto r: usethis)
                {
                    long cangive = max/nct[r];  //cangive is the number of jobs that can be given to this resource according to proportion
                    cangive = cangive < (10-assigned[r].size()) ? cangive : (10-(assigned[r].size()));  //each resource can take maximum 10 jobs
                    long add2=(Qcosts[r]*90*cangive);
                    /*allot the jobs*/
                    if(cost+add2<budget[j])
                    {
                        cost+=add2;
                    }
                    else
                    {
                        add2 = Qcosts[r]*90;
                        while(cost<budget[j]) cost+=add2;
                        cost-=add2;
                        flag = true;
                        break;
                    }
                    /*stores which job is assigned to which resource*/
                    for(int i=0; i<cangive; ++i)
                    {
                        assigned[r].push_back(k+i);
                        totassign[r]++;
                    }
                    t+=cangive;
                }
                
                /*if no job satisfies the criterias of budget : break*/
                if(usethis.empty())
                {
                    break;
                }
                if(flag)    break;
                /*assign job to first available cheapest resource if job not given to any resource yet*/
                if(t==0)
                {
                    int i = 0;
                    while(assigned[i].size()==10)   i++;
                    assigned[i].push_back(k+1);
                    cost+=90*Qcosts[i];
                    totassign[i]++;
                    t++;
                }
                k+=t;
                
                /*check that deadline is not exceeded*/
                max = -1;
                REP(r, 10)
                {
                    if(totassign[r]>max)
                        max = totassign[r];
                }
                if(max*90 > (deadline[i]*90)/100)    break;
                
            }
            cout<<deadline[i]<<"\t"<<budget[j]<<"\t"<<cost<<"\t"<<endl;
            
        }
    }
    
    return 0;
}












