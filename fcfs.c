    #include "scheduler.h"
    #include <string.h>


    int schedule_fcfs(Process *procs, int n, TimelineEvent *timeline) {
        int current_time = 0;     
        int timeline_idx = 0;     // Track Gantt chart events
        
        for (int i = 0; i < n; i++) {
            // Handle idle time (if process hasn't arrived yet)
            if (current_time < procs[i].arrival_time) {
                current_time = procs[i].arrival_time;
            }
            
            // Record when process first starts
            if (procs[i].start_time == -1) {
                procs[i].start_time = current_time;
            }
        
            
            // Create timeline event 
            timeline[timeline_idx].pid = procs[i].pid;
            timeline[timeline_idx].start = current_time;
            timeline[timeline_idx].end = current_time + procs[i].burst_time;
            strcpy(timeline[timeline_idx].state, "running");
            timeline_idx++;
            

            current_time += procs[i].burst_time;
            
            // Record completion time 
            procs[i].completion_time = current_time;
            
        
        }
        
        return timeline_idx;
    }