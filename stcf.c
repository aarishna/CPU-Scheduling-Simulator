#include <stdio.h>
#define max 10

void stcf(int burst[], int n, int arrival[]){

    int remaining[max],completion[max],waiting[max],turnaround[max];
    int time=0;
    int completed=0;
    int total_waiting=0;
    int total_turnaround=0;
        for (int i=0; i<n; i++){
            remaining[i]=burst[i];
        }
        while (completed<n){
            int current=-1;
            for (int i = 0; i < n; i++) {
            if (arrival[i] <= time && remaining[i] > 0) {
                if (current == -1) {
                    current = i;
                } else if (remaining[i] < remaining[current]) {
                    current = i;
                }
            }
        }
        if (current ==-1){
            time++;
            continue;
        }
        remaining[current]--;
        time++;

        if (remaining[current]==0){
            completed++;
            completion[current]=time;
            turnaround[current]= completion[current]- arrival[current];
            waiting[current]= turnaround[current]- burst[current];

            total_turnaround+=turnaround[current];
            total_waiting+=waiting[current];
        }
    }
    printf("\nProcess No.\tArrival Time\tBurst Time\tCompletion Time\t Turnaround Time\tWaiting Time\n");

    for (int i=0; i<n; i++){
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t %d\t\t \t%d\n",(i+1),arrival[i],burst[i],completion[i],turnaround[i],waiting[i] );
    } 
    printf("\nAverage Waiting Time: %.2f", (float)total_waiting / n);
    printf("\nAverage Turnaround Time: %.2f\n", (float)total_turnaround / n);
}

    
int main(){
    int burst_time[max], arrival[max];
    int n;
    
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    printf("Enter the burst times of each process:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("Enter the arrival time of process %d: ", (i + 1));
        scanf("%d", &arrival[i]);
        printf("Enter the burst time of process %d: ", (i + 1));
        scanf("%d", &burst_time[i]);
        
    }
    
    stcf(burst_time, n, arrival);
    
    printf("\n");
    
    return 0;
}

    
