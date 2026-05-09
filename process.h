#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

typedef struct {
    int pid;                    // Process ID
    int arrival_time;           // When process arrives
    int burst_time;             // Total CPU time needed
    int priority;               // Priority (lower number = higher priority)
    
    // Calculated metrics
    int remaining_time;         // Time left to execute (for preemptive)
    int completion_time;        // When process finishes
    int turnaround_time;        // completion_time - arrival_time
    int waiting_time;           // turnaround_time - burst_time
    int response_time;          // first_run_time - arrival_time
    int first_run_time;         // When process first gets CPU
    
    // For MLFQ
    int queue_level;            // Current queue level (0 = highest priority)
} Process;

// Gantt chart entry
typedef struct {
    int pid;
    int start_time;
    int end_time;
} GanttEntry;

// Gantt chart structure
typedef struct {
    GanttEntry *entries;
    int count;
    int capacity;
} GanttChart;

// Scheduling metrics
typedef struct {
    double avg_waiting_time;
    double avg_turnaround_time;
    double avg_response_time;
    double cpu_utilization;
    double throughput;
    int total_time;
} SchedulingMetrics;

// Function declarations
void init_process(Process *p, int pid, int arrival, int burst, int priority);
GanttChart* create_gantt_chart();
void add_gantt_entry(GanttChart *chart, int pid, int start, int end);
void free_gantt_chart(GanttChart *chart);
void print_gantt_chart(GanttChart *chart);

#endif // PROCESS_H
