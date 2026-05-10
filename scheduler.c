#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"

void init_process(Process *p, int pid, int arrival, int burst, int priority) {
    p->pid = pid;
    p->arrival_time = arrival;
    p->burst_time = burst;
    p->priority = priority;
    p->remaining_time = burst;
    p->completion_time = 0;
    p->turnaround_time = 0;
    p->waiting_time = 0;
    p->response_time = 0;
    p->first_run_time = -1;
    p->queue_level = 0;
}

GanttChart* create_gantt_chart() {
    GanttChart *chart = (GanttChart*)malloc(sizeof(GanttChart));
    chart->capacity = 100;
    chart->count = 0;
    chart->entries = (GanttEntry*)malloc(chart->capacity * sizeof(GanttEntry));
    return chart;
}

void add_gantt_entry(GanttChart *chart, int pid, int start, int end) {
    // Merge with previous entry if same process
    if (chart->count > 0 && 
        chart->entries[chart->count - 1].pid == pid &&
        chart->entries[chart->count - 1].end_time == start) {
        chart->entries[chart->count - 1].end_time = end;
        return;
    }
    
    // Expand if needed
    if (chart->count >= chart->capacity) {
        chart->capacity *= 2;
        chart->entries = (GanttEntry*)realloc(chart->entries, 
                                               chart->capacity * sizeof(GanttEntry));
    }
    
    chart->entries[chart->count].pid = pid;
    chart->entries[chart->count].start_time = start;
    chart->entries[chart->count].end_time = end;
    chart->count++;
}

void free_gantt_chart(GanttChart *chart) {
    free(chart->entries);
    free(chart);
}

void print_gantt_chart(GanttChart *chart) {
    printf("\n=== Gantt Chart ===\n");
    printf("| ");
    for (int i = 0; i < chart->count; i++) {
        printf("P%d | ", chart->entries[i].pid);
    }
    printf("\n");
    
    printf("%d", chart->entries[0].start_time);
    for (int i = 0; i < chart->count; i++) {
        printf("    %d", chart->entries[i].end_time);
    }
    printf("\n");
}

void calculate_metrics(Process *processes, int n, int total_time, SchedulingMetrics *metrics) {
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_response = 0;
    int total_burst = 0;
    int min_arrival = processes[0].arrival_time;
    int max_completion = processes[0].completion_time;
    
    for (int i = 0; i < n; i++) {
        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
        total_response += processes[i].response_time;
        total_burst += processes[i].burst_time;
        
        if (processes[i].arrival_time < min_arrival) {
            min_arrival = processes[i].arrival_time;
        }
        if (processes[i].completion_time > max_completion) {
            max_completion = processes[i].completion_time;
        }
    }
    
    total_time = max_completion - min_arrival;
    
    metrics->avg_waiting_time = (double)total_waiting / n;
    metrics->avg_turnaround_time = (double)total_turnaround / n;
    metrics->avg_response_time = (double)total_response / n;
    metrics->cpu_utilization = (double)total_burst / total_time * 100.0;
    metrics->throughput = (double)n / total_time;
    metrics->total_time = total_time;
}

void print_metrics(SchedulingMetrics *metrics) {
    printf("\n=== Scheduling Metrics ===\n");
    printf("Average Waiting Time:     %.2f\n", metrics->avg_waiting_time);
    printf("Average Turnaround Time:  %.2f\n", metrics->avg_turnaround_time);
    printf("Average Response Time:    %.2f\n", metrics->avg_response_time);
    printf("CPU Utilization:          %.2f%%\n", metrics->cpu_utilization);
    printf("Throughput:               %.4f processes/unit time\n", metrics->throughput);
    printf("Total Time:               %d\n", metrics->total_time);
}

void print_process_table(Process *processes, int n) {
    printf("\n=== Process Table ===\n");
    printf("PID | Arrival | Burst | Completion | Turnaround | Waiting | Response\n");
    printf("----+---------+-------+------------+------------+---------+---------\n");
    
    for (int i = 0; i < n; i++) {
        printf("P%-2d |   %3d   |  %3d  |    %3d     |    %3d     |   %3d   |   %3d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time,
               processes[i].response_time);
    }
}

int load_processes_from_file(const char *filename, Process **processes) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }
    
    int n;
    fscanf(file, "%d", &n);
    
    *processes = (Process*)malloc(n * sizeof(Process));
    
    for (int i = 0; i < n; i++) {
        int pid, arrival, burst, priority;
        fscanf(file, "%d %d %d %d", &pid, &arrival, &burst, &priority);
        init_process(&(*processes)[i], pid, arrival, burst, priority);
    }
    
    fclose(file);
    return n;
}

void generate_random_processes(Process **processes, int n, int max_arrival, int max_burst) {
    *processes = (Process*)malloc(n * sizeof(Process));
    
    for (int i = 0; i < n; i++) {
        int arrival = rand() % max_arrival;
        int burst = (rand() % max_burst) + 1;
        int priority = rand() % 10;
        init_process(&(*processes)[i], i + 1, arrival, burst, priority);
    }
}

void output_to_json(Process *processes, int n, GanttChart *chart, 
                    const char *algorithm, SchedulingMetrics *metrics, 
                    const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Cannot create output file\n");
        return;
    }
    
    fprintf(file, "{\n");
    fprintf(file, "  \"algorithm\": \"%s\",\n", algorithm);
    fprintf(file, "  \"num_processes\": %d,\n", n);
    
    // Metrics
    fprintf(file, "  \"metrics\": {\n");
    fprintf(file, "    \"avg_waiting_time\": %.2f,\n", metrics->avg_waiting_time);
    fprintf(file, "    \"avg_turnaround_time\": %.2f,\n", metrics->avg_turnaround_time);
    fprintf(file, "    \"avg_response_time\": %.2f,\n", metrics->avg_response_time);
    fprintf(file, "    \"cpu_utilization\": %.2f,\n", metrics->cpu_utilization);
    fprintf(file, "    \"throughput\": %.4f,\n", metrics->throughput);
    fprintf(file, "    \"total_time\": %d\n", metrics->total_time);
    fprintf(file, "  },\n");
    
    // Processes
    fprintf(file, "  \"processes\": [\n");
    for (int i = 0; i < n; i++) {
        fprintf(file, "    {\n");
        fprintf(file, "      \"pid\": %d,\n", processes[i].pid);
        fprintf(file, "      \"arrival_time\": %d,\n", processes[i].arrival_time);
        fprintf(file, "      \"burst_time\": %d,\n", processes[i].burst_time);
        fprintf(file, "      \"completion_time\": %d,\n", processes[i].completion_time);
        fprintf(file, "      \"turnaround_time\": %d,\n", processes[i].turnaround_time);
        fprintf(file, "      \"waiting_time\": %d,\n", processes[i].waiting_time);
        fprintf(file, "      \"response_time\": %d\n", processes[i].response_time);
        fprintf(file, "    }%s\n", (i < n - 1) ? "," : "");
    }
    fprintf(file, "  ],\n");
    
    // Gantt chart
    fprintf(file, "  \"gantt_chart\": [\n");
    for (int i = 0; i < chart->count; i++) {
        fprintf(file, "    {\n");
        fprintf(file, "      \"pid\": %d,\n", chart->entries[i].pid);
        fprintf(file, "      \"start\": %d,\n", chart->entries[i].start_time);
        fprintf(file, "      \"end\": %d\n", chart->entries[i].end_time);
        fprintf(file, "    }%s\n", (i < chart->count - 1) ? "," : "");
    }
    fprintf(file, "  ]\n");
    
    fprintf(file, "}\n");
    
    fclose(file);
    printf("\nOutput written to %s\n", filename);
}