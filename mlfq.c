#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "process.h"
#include "scheduler.h"
#include "queue.h"

#define NUM_QUEUES 3
#define BOOST_INTERVAL 100

void mlfq_schedule(Process *processes, int n, GanttChart *chart) {
    int current_time = 0;
    int completed = 0;
    int last_boost_time = 0;
    
    int time_quantums[NUM_QUEUES] = {8, 16, INT_MAX};
    
    Queue *queues[NUM_QUEUES];
    for (int i = 0; i < NUM_QUEUES; i++) {
        queues[i] = create_queue(n * 10);
    }
    
    bool *in_any_queue = calloc(n, sizeof(bool));
    
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].queue_level = 0;
    }
    
    printf("\n=== MLFQ Scheduling ===\n");
    printf("Queue 0: Time Quantum = %d (Highest Priority)\n", time_quantums[0]);
    printf("Queue 1: Time Quantum = %d\n", time_quantums[1]);
    printf("Queue 2: FCFS (Lowest Priority)\n");
    printf("Priority Boost Interval: %d time units\n\n", BOOST_INTERVAL);
    
    while (completed < n) {
        if (current_time - last_boost_time >= BOOST_INTERVAL && current_time > 0) {
            printf("Time %d: PRIORITY BOOST - All processes moved to Queue 0\n", 
                   current_time);
            
            int temp_processes[n];
            int temp_count = 0;
            
            for (int i = 0; i < NUM_QUEUES; i++) {
                while (!is_empty(queues[i])) {
                    int idx = dequeue(queues[i]);
                    temp_processes[temp_count++] = idx;
                    processes[idx].queue_level = 0;
                }
            }
            
            for (int i = 0; i < temp_count; i++) {
                enqueue(queues[0], temp_processes[i]);
            }
            
            last_boost_time = current_time;
        }
        
        for (int i = 0; i < n; i++) {
            if (!in_any_queue[i] && processes[i].arrival_time == current_time &&
                processes[i].remaining_time > 0) {
                enqueue(queues[0], i);
                in_any_queue[i] = true;
                processes[i].queue_level = 0;
                printf("Time %d: Process P%d arrives (added to Queue 0)\n", 
                       current_time, processes[i].pid);
            }
        }
        
        int active_queue = -1;
        for (int i = 0; i < NUM_QUEUES; i++) {
            if (!is_empty(queues[i])) {
                active_queue = i;
                break;
            }
        }
        
        if (active_queue == -1) {
            current_time++;
            continue;
        }
        
        int idx = dequeue(queues[active_queue]);
        in_any_queue[idx] = false;
        Process *p = &processes[idx];
        
        if (p->first_run_time == -1) {
            p->first_run_time = current_time;
            p->response_time = current_time - p->arrival_time;
            printf("Time %d: Process P%d starts for the first time (Queue %d)\n", 
                   current_time, p->pid, active_queue);
        }
        
        int quantum = time_quantums[active_queue];
        int exec_time = (p->remaining_time < quantum) ? p->remaining_time : quantum;
        
        add_gantt_entry(chart, p->pid, current_time, current_time + exec_time);
        
        p->remaining_time -= exec_time;
        int start_time = current_time;
        current_time += exec_time;
        
        printf("Time %d-%d: Process P%d executes in Queue %d (Remaining: %d)\n", 
               start_time, current_time, p->pid, active_queue, p->remaining_time);
        
        for (int i = 0; i < n; i++) {
            if (!in_any_queue[i] && i != idx &&
                processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0) {
                enqueue(queues[0], i);
                in_any_queue[i] = true;
                processes[i].queue_level = 0;
                printf("Time %d: Process P%d arrives (added to Queue 0)\n", 
                       current_time, processes[i].pid);
            }
        }
        
        if (p->remaining_time == 0) {
            p->completion_time = current_time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            
            printf("Time %d: Process P%d completed (Turnaround: %d, Waiting: %d)\n", 
                   current_time, p->pid, p->turnaround_time, p->waiting_time);
            completed++;
        } else {
            if (exec_time == quantum && active_queue < NUM_QUEUES - 1) {
                p->queue_level++;
                enqueue(queues[active_queue + 1], idx);
                printf("Time %d: Process P%d demoted to Queue %d\n", 
                       current_time, p->pid, active_queue + 1);
            } else {
                enqueue(queues[active_queue], idx);
            }
            in_any_queue[idx] = true;
        }
    }
    
    for (int i = 0; i < NUM_QUEUES; i++) {
        free_queue(queues[i]);
    }
    free(in_any_queue);
    
    printf("MLFQ scheduling completed at time %d\n", current_time);
}