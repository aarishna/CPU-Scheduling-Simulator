#include "process.h"
#include <string.h>



void init_process(Process *p, int pid, int arrival, int burst) {
    p->pid = pid; 
    p->arrival_time = arrival;
    p->burst_time = burst;
    p->remaining_time = burst;  
    p->start_time = -1;
    p->completion_time = -1; 
}

int turnaround_time(Process *p) {
    return p->completion_time - p->arrival_time;
}
int waiting_time(Process *p) {
    return turnaround_time(p) - p->burst_time;
}
int response_time(Process *p) {
    return p->start_time - p->arrival_time;
}
/* Print metrics */
void print_metrics(Process *procs, int n) {
    printf("\n==================================================================\n");
    printf("||  PROCESS EXECUTION RESULTS  \n");
    printf(" ===================================================================\n");
    printf("|| PID  Arrival  Burst  Start  Complete  Wait  Turnaround  Response \n");
    printf(" ===================================================================\n");
    
    float total_wait = 0, total_tat = 0, total_response = 0;
    
    for (int i = 0; i < n; i++) {
        int wt = waiting_time(&procs[i]);
        int tat = turnaround_time(&procs[i]);
        int rt = response_time(&procs[i]);
        
        printf("|| %-4d %-8d %-6d %-6d %-9d %-5d %-11d %-7d\n",
               procs[i].pid,
               procs[i].arrival_time,
               procs[i].burst_time,
               procs[i].start_time,
               procs[i].completion_time,
               wt, tat, rt);
        
        total_wait += wt;
        total_tat += tat;
        total_response += rt;
    }
    
    printf(" =================================================================\n");
    printf("||  AVERAGE METRICS:\n");
    printf("||    Waiting Time:      %.2f time units\n", total_wait / n);
    printf("||    Turnaround Time:   %.2f time units\n", total_tat / n);
    printf("||    Response Time:     %.2f time units\n", total_response / n);
    printf(" =================================================================\n");
}

/* Generate JSON for Python visualizer */
void generate_json(const char *algorithm, Process *procs, int n, 
                   TimelineEvent *timeline, int timeline_size, 
                   const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Could not create file %s\n", filename);
        return;
    }
    
    // Calculate average metrics
    float total_wait = 0, total_tat = 0, total_response = 0;
    int total_time = 0;
    
    for (int i = 0; i < n; i++) {
        total_wait += waiting_time(&procs[i]);
        total_tat += turnaround_time(&procs[i]);
        total_response += response_time(&procs[i]);
        if (procs[i].completion_time > total_time) {
            total_time = procs[i].completion_time;
        }
    }
    
    // Write JSON
    fprintf(fp, "{\n");
    fprintf(fp, "  \"algorithm\": \"%s\",\n", algorithm);
    fprintf(fp, "  \"num_processes\": %d,\n", n);
    
    // Processes array
    fprintf(fp, "  \"processes\": [\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "    {\n");
        fprintf(fp, "      \"pid\": %d,\n", procs[i].pid);
        fprintf(fp, "      \"arrival_time\": %d,\n", procs[i].arrival_time);
        fprintf(fp, "      \"burst_time\": %d,\n", procs[i].burst_time);
        fprintf(fp, "      \"start_time\": %d,\n", procs[i].start_time);
        fprintf(fp, "      \"completion_time\": %d,\n", procs[i].completion_time);
        fprintf(fp, "      \"waiting_time\": %d,\n", waiting_time(&procs[i]));
        fprintf(fp, "      \"turnaround_time\": %d,\n", turnaround_time(&procs[i]));
        fprintf(fp, "      \"response_time\": %d\n", response_time(&procs[i]));
        fprintf(fp, "    }%s\n", (i < n - 1) ? "," : "");
    }
    fprintf(fp, "  ],\n");
    
    // Timeline array (for Gantt chart)
    fprintf(fp, "  \"timeline\": [\n");
    for (int i = 0; i < timeline_size; i++) {
        fprintf(fp, "    {\n");
        fprintf(fp, "      \"pid\": %d,\n", timeline[i].pid);
        fprintf(fp, "      \"start\": %d,\n", timeline[i].start);
        fprintf(fp, "      \"end\": %d,\n", timeline[i].end);
        fprintf(fp, "      \"state\": \"%s\"\n", timeline[i].state);
        fprintf(fp, "    }%s\n", (i < timeline_size - 1) ? "," : "");
    }
    fprintf(fp, "  ],\n");
    
    // Metrics
    fprintf(fp, "  \"metrics\": {\n");
    fprintf(fp, "    \"avg_waiting_time\": %.2f,\n", total_wait / n);
    fprintf(fp, "    \"avg_turnaround_time\": %.2f,\n", total_tat / n);
    fprintf(fp, "    \"avg_response_time\": %.2f,\n", total_response / n);
    fprintf(fp, "    \"cpu_utilization\": 100.0,\n");
    fprintf(fp, "    \"throughput\": %.4f\n", (float)n / total_time);
    fprintf(fp, "  }\n");
    
    fprintf(fp, "}\n");
    fclose(fp);
    
    printf(" JSON output saved to: %s\n", filename);
}