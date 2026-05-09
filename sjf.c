#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "process.h"
#include "scheduler.h"

void sjf_schedule(Process *processes, int n, GanttChart *chart) {
    int current_time = 0;
    int completed = 0;
    bool *is_completed = calloc(n, sizeof(bool));

    printf("\n=== SJF Scheduling ===\n");

    while (completed < n) {
        int shortest_idx = -1;
        int shortest_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!is_completed[i] &&
                processes[i].arrival_time <= current_time &&
                processes[i].burst_time < shortest_burst) {
                shortest_burst = processes[i].burst_time;
                shortest_idx = i;
            }
        }

        if (shortest_idx == -1) {
            current_time++;
            continue;
        }

        Process *p = &processes[shortest_idx];

        if (p->first_run_time == -1) {
            p->first_run_time = current_time;
            p->response_time = current_time - p->arrival_time;
        }

        add_gantt_entry(chart, p->pid, current_time, current_time + p->burst_time);

        current_time += p->burst_time;
        p->completion_time = current_time;
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;

        printf("Time %d: Process P%d completed (Burst: %d, Waiting: %d)\n",
               current_time, p->pid, p->burst_time, p->waiting_time);

        is_completed[shortest_idx] = true;
        completed++;
    }

    free(is_completed);
    printf("SJF scheduling completed at time %d\n", current_time);
}
