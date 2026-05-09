#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#include "scheduler.h"
#include "queue.h"

void rr_schedule(Process *processes, int n, int time_quantum, GanttChart *chart) {
    int current_time = 0;
    int completed = 0;

    Queue *ready_queue = create_queue(n * 10);
    bool *in_queue = calloc(n, sizeof(bool));

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    printf("\n=== Round Robin Scheduling (Quantum: %d) ===\n", time_quantum);

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time == 0) {
            enqueue(ready_queue, i);
            in_queue[i] = true;
            printf("Time %d: Process P%d added to ready queue\n",
                   current_time, processes[i].pid);
        }
    }

    while (completed < n) {
        if (is_empty(ready_queue)) {
            current_time++;

            for (int i = 0; i < n; i++) {
                if (!in_queue[i] &&
                    processes[i].arrival_time <= current_time &&
                    processes[i].remaining_time > 0) {
                    enqueue(ready_queue, i);
                    in_queue[i] = true;
                    printf("Time %d: Process P%d added to ready queue\n",
                           current_time, processes[i].pid);
                }
            }

            continue;
        }

        int idx = dequeue(ready_queue);
        in_queue[idx] = false;
        Process *p = &processes[idx];

        if (p->first_run_time == -1) {
            p->first_run_time = current_time;
            p->response_time = current_time - p->arrival_time;
            printf("Time %d: Process P%d starts for the first time\n",
                   current_time, p->pid);
        }

        int exec_time = (p->remaining_time < time_quantum)
                            ? p->remaining_time
                            : time_quantum;

        add_gantt_entry(chart, p->pid, current_time, current_time + exec_time);

        p->remaining_time -= exec_time;
        int start_time = current_time;
        current_time += exec_time;

        printf("Time %d-%d: Process P%d executes (Remaining: %d)\n",
               start_time, current_time, p->pid, p->remaining_time);

        for (int i = 0; i < n; i++) {
            if (!in_queue[i] &&
                i != idx &&
                processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0) {
                enqueue(ready_queue, i);
                in_queue[i] = true;
                printf("Time %d: Process P%d added to ready queue\n",
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
            enqueue(ready_queue, idx);
            in_queue[idx] = true;
        }
    }

    free_queue(ready_queue);
    free(in_queue);

    printf("Round Robin scheduling completed at time %d\n", current_time);
}
