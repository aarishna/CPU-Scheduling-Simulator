#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"
#include "scheduler.h"

void print_usage(const char *prog_name) {
    printf("Usage: %s <algorithm> [options]\n", prog_name);
    printf("\nAlgorithms:\n");
    printf("  fcfs           - First Come First Served\n");
    printf("  sjf            - Shortest Job First\n");
    printf("  stcf           - Shortest Time-to-Completion First\n");
    printf("  rr <quantum>   - Round Robin with time quantum\n");
    printf("  mlfq           - Multi-Level Feedback Queue\n");
    printf("  all            - Run all algorithms for comparison\n");
    printf("\nOptions:\n");
    printf("  -f <file>      - Load processes from file\n");
    printf("  -r <n>         - Generate n random processes\n");
    printf("  -o <file>      - Output JSON file (default: output.json)\n");
    printf("\nExamples:\n");
    printf("  %s fcfs -f processes.txt\n", prog_name);
    printf("  %s rr 4 -r 10\n", prog_name);
    printf("  %s all -f processes.txt -o results.json\n", prog_name);
}

void run_algorithm(const char *algo_name, Process *processes, int n, 
                   int time_quantum, const char *output_file) {
    GanttChart *chart = create_gantt_chart();
    SchedulingMetrics metrics;
    
    printf("\n========================================\n");
    printf("Running %s Algorithm\n", algo_name);
    printf("========================================\n");
    
    if (strcmp(algo_name, "FCFS") == 0) {
        fcfs_schedule(processes, n, chart);
    } else if (strcmp(algo_name, "SJF") == 0) {
        sjf_schedule(processes, n, chart);
    } else if (strcmp(algo_name, "STCF") == 0) {
        stcf_schedule(processes, n, chart);
    } else if (strcmp(algo_name, "RR") == 0) {
        rr_schedule(processes, n, time_quantum, chart);
    } else if (strcmp(algo_name, "MLFQ") == 0) {
        mlfq_schedule(processes, n, chart);
    }
    
    calculate_metrics(processes, n, 0, &metrics);
    print_process_table(processes, n);
    print_gantt_chart(chart);
    print_metrics(&metrics);
    
    if (output_file) {
        output_to_json(processes, n, chart, algo_name, &metrics, output_file);
    }
    
    free_gantt_chart(chart);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    char *algorithm = argv[1];
    int time_quantum = 4;  // Default for RR
    char *input_file = NULL;
    char *output_file = "output.json";
    int random_count = 0;
    Process *processes = NULL;
    int n = 0;
    
    // Parse arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            random_count = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(algorithm, "rr") == 0 && i == 2) {
            time_quantum = atoi(argv[i]);
        }
    }
    
    // Load or generate processes
    if (input_file) {
        n = load_processes_from_file(input_file, &processes);
        if (n <= 0) {
            printf("Error loading processes from file\n");
            return 1;
        }
        printf("Loaded %d processes from %s\n", n, input_file);
    } else if (random_count > 0) {
        srand(time(NULL));
        n = random_count;
        generate_random_processes(&processes, n, 20, 15);
        printf("Generated %d random processes\n", n);
    } else {
        printf("Error: Must specify either -f <file> or -r <count>\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Run algorithm(s)
    // Run algorithm(s)
if (strcmp(algorithm, "all") == 0) {
    // Run all algorithms
    const char *algos[] = {"FCFS", "SJF", "STCF", "RR", "MLFQ"};
    
    for (int i = 0; i < 5; i++) {
        // Create a copy of processes for each algorithm
        Process *proc_copy = (Process*)malloc(n * sizeof(Process));
        memcpy(proc_copy, processes, n * sizeof(Process));
        
        // Create output filename safely without modifying output_file
        char out_file[256];
        char base_name[256];
        strcpy(base_name, output_file);
        
        // Find the last dot for extension
        char *dot = strrchr(base_name, '.');
        if (dot != NULL) {
            *dot = '\0';  // Remove extension
        }
        
        // Create filename: basename_ALGORITHM.json
        snprintf(out_file, sizeof(out_file), "%s_%s.json", base_name, algos[i]);
        
        run_algorithm(algos[i], proc_copy, n, time_quantum, out_file);
        free(proc_copy);
        }
    } else {
        // Run single algorithm
        char *algo_upper = strdup(algorithm);
        for (char *p = algo_upper; *p; p++) {
            if (*p >= 'a' && *p <= 'z') *p = *p - 'a' + 'A';
        }
        
        run_algorithm(algo_upper, processes, n, time_quantum, output_file);
        free(algo_upper);
    }
    
    free(processes);
    
    printf("\n========================================\n");
    printf("Scheduling simulation completed!\n");
    printf("========================================\n");
    
    return 0;
}
 