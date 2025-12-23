#include <stdio.h>
#define max 10
int round_robin(int burst_time[], int n, int quantum){
    int remaining[max];
    int waiting_time= 0; 
    int turnaround_time= 0;

    for (int i=0; i<n; i++){
        remaining[i]=burst_time[i];
    }
    int current_time = 0; 
    int completed =0;

    printf("Process No.\t Burst time \t Waiting time\t Turnaround time\n");
    while (completed< n)
    {
        int all_done = 1 ;

        for (int i=0; i<n; i++){
            if (remaining[i]>0){
                all_done=0;

                if (remaining[i]<=quantum){
                    current_time+=remaining[i];
                    int turnaround = current_time;
                    int waiting = turnaround - burst_time[i];
                    printf("P%d\t\t %d\t\t %d\t\t %d\n", i + 1, burst_time[i], waiting, turnaround);
                    
                    waiting_time += waiting;
                    turnaround_time += turnaround;
                    remaining[i] = 0;
                    completed++;
                }
                else{
                    current_time +=quantum;
                    remaining[i]-=quantum;
                }
            }
        }
        if (all_done){break;}

        
    printf("Average turnaround time: %.2f\n", (float) turnaround_time / n);
    printf("Average waiting time: %.2f", (float) waiting_time / n);
}}
    


int main (){
    int n, bt[max],quantum;

    printf("Enter the number of processes (MAX 10): ");
    scanf("%d",&n);

    printf("Enter burst time of each process \n\n");
    for (int i=0; i < n ; i++){
        printf("Enter burst time of process %d: ", i);
        scanf("%d", &bt[i]);
    }
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
    round_robin(bt,n,quantum);
    return 0;
}
