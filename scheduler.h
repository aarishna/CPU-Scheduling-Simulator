#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

// Scheduling algorithms
void fcfs_schedule(Process *processes, int n, GanttChart *chart);
void sjf_schedule(Process *processes, int n, GanttChart *chart);
void stcf_schedule(Process *processes, int n, GanttChart *chart);
void rr_schedule(Process *processes, int n, int time_quantum, GanttChart *chart);
void mlfq_schedule(Process *processes, int n, GanttChart *chart);

// Utility functions
void calculate_metrics(Process *processes, int n, int total_time, SchedulingMetrics *metrics);
void print_metrics(SchedulingMetrics *metrics);
void print_process_table(Process *processes, int n);
int load_processes_from_file(const char *filename, Process **processes);
void generate_random_processes(Process **processes, int n, int max_arrival, int max_burst);
void output_to_json(Process *processes, int n, GanttChart *chart, 
                    const char *algorithm, SchedulingMetrics *metrics, const char *filename);

#endif // SCHEDULER_H