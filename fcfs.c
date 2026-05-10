#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "scheduler.h"

void fcfs_schedule(Process *processes, int n, GanttChart *chart) {
    int current_time = 0;
    
    printf("\n=== FCFS Scheduling ===\n");
    
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            printf("Time %d-%d: CPU idle\n", current_time, processes[i].arrival_time);
            current_time = processes[i].arrival_time;
        }
        
        if (processes[i].first_run_time == -1) {
            processes[i].first_run_time = current_time;
            processes[i].response_time = current_time - processes[i].arrival_time;
        }
        
        printf("Time %d: Process P%d starts (Burst: %d)\n", 
               current_time, processes[i].pid, processes[i].burst_time);
        
        add_gantt_entry(chart, processes[i].pid, current_time, 
                       current_time + processes[i].burst_time);
        
        current_time += processes[i].burst_time;
        
        processes[i].completion_time = current_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        
        printf("Time %d: Process P%d completed (Waiting: %d, Turnaround: %d)\n", 
               current_time, processes[i].pid, processes[i].waiting_time, 
               processes[i].turnaround_time);
    }
    
    printf("FCFS scheduling completed at time %d\n", current_time);
}