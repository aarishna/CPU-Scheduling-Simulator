#include <stdio.h>
#define max 10

void sjf(int pid[], int bt[], int n){
    int waiting_time = 0;
    int turnaround_time = 0; 
    int total =0;
    for (int i=0; i< n-1; i++){
        for (int j=0; j< n-i-1; j++){
            if (bt[j]>bt[j+1]){
                int temp = bt[j];
                bt[j]=bt[j+1];
                bt[j+1]=temp;

                temp=pid[j];
                pid[j]=pid[j+1];
                pid[j+1]=temp;

            }
        }
    }
     printf("\nProcess No.\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i)
    {
        total += bt[i];
        
        printf("%d\t\t%d\t\t%d\n", pid[i], (total - bt[i]), total);
        
        waiting_time += (total - bt[i]);
        turnaround_time += total;
    }
    printf("\n");    
    printf("The average waiting time is: %.2f\n", ((float) waiting_time / n));
    printf("The average turnaround time is: %.2f\n", ((float) turnaround_time / n));
}
int main(void)
{
    int burst_time[max], pid[max];
    int n;
    
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    printf("Enter the burst times of each process:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("Enter the burst time of process %d: ", (i + 1));
        scanf("%d", &burst_time[i]);
        
        pid[i] = (i + 1);
    }
    
    sjf(pid, burst_time, n);
    
    printf("\n");
    
    return 0;
}
