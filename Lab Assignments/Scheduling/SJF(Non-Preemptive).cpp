#include<bits/stdc++.h>
using namespace std;
 
struct Process {
   int pid; // Process ID
   int bt;  // Burst Time
};

bool comparison(Process a, Process b) {
     return (a.bt < b.bt);
}

void findWaitingTime(Process proc[], int n, int wt[]) {
    wt[0] = 0;

    for (int i = 1; i < n ; i++ )
        wt[i] = proc[i-1].bt + wt[i-1] ;
}

void findTurnAroundTime(Process proc[], int n, int wt[], int tat[]) {
    for (int i = 0; i < n ; i++)
        tat[i] = proc[i].bt + wt[i];
}

void findavgTime(Process proc[], int n) {
    int wt[n], tat[n], total_wt = 0, total_tat = 0;
 
    findWaitingTime(proc, n, wt);
 
    findTurnAroundTime(proc, n, wt, tat);

    cout << "\nProcesses "<< " Burst time "
         << " Waiting time " << " Turn around time\n";
 
    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        cout<< " " << proc[i].pid << "\t\t"
             << proc[i].bt << "\t " << wt[i]
             << "\t\t " << tat[i] <<endl;
    }
 
    cout<<"Average waiting time = "
         << (float)total_wt / (float)n;
    cout<<"\nAverage turn around time = "
         << (float)total_tat / (float)n;
    cout<<endl;
}
 

int main() {
    Process proc[] = {{1, 6}, {2, 8}, {3, 7}, {4, 3}};
    int n = sizeof proc / sizeof proc[0];
 
    sort(proc, proc + n, comparison);
 
    cout<< "Order in which process gets executed\n";
    for (int i = 0 ; i < n; i++)
        cout<<proc[i].pid <<" ";
 
    findavgTime(proc, n);
    return 0;
}