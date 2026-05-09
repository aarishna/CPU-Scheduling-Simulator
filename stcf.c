#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "process.h"
#include "scheduler.h"

void stcf_schedule(Process *processes, int n, GanttChart *chart) {
    int current_time = 0;
    int completed = 0;
    int last_pid = -1;
    int segment_start = 0;
    
    // Initialize remaining times
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    printf("\n=== STCF Scheduling (Preemptive SJF) ===\n");
    
    while (completed < n) {
        int shortest_idx = -1;
        int shortest_remaining = INT_MAX;
        
        // Find process with shortest remaining time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0 &&
                processes[i].remaining_time < shortest_remaining) {
                shortest_remaining = processes[i].remaining_time;
                shortest_idx = i;
            }
        }
        
        // If no process is ready, advance time
        if (shortest_idx == -1) {
            current_time++;
            continue;
        }
        
        Process *p = &processes[shortest_idx];
        
        // Record response time on first run
        if (p->first_run_time == -1) {
            p->first_run_time = current_time;
            p->response_time = current_time - p->arrival_time;
            printf("Time %d: Process P%d starts (Remaining: %d)\n", 
                   current_time, p->pid, p->remaining_time);
        }
        
        // If switching processes, save previous Gantt entry
        if (last_pid != -1 && last_pid != p->pid) {
            add_gantt_entry(chart, last_pid, segment_start, current_time);
            printf("Time %d: Context switch from P%d to P%d\n", 
                   current_time, last_pid, p->pid);
            segment_start = current_time;
        } else if (last_pid == -1) {
            segment_start = current_time;
        }
        
        // Execute for 1 time unit
        p->remaining_time--;
        current_time++;
        last_pid = p->pid;
        
        // Check if process completed
        if (p->remaining_time == 0) {
            add_gantt_entry(chart, p->pid, segment_start, current_time);
            p->completion_time = current_time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            
            printf("Time %d: Process P%d completed (Turnaround: %d, Waiting: %d)\n", 
                   current_time, p->pid, p->turnaround_time, p->waiting_time);
            
            completed++;
            last_pid = -1;
        }
    }
    
    printf("STCF scheduling completed at time %d\n", current_time);
}
