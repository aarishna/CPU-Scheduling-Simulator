#include <stdio.h>
#define num_queues 5
#define max_processes 5

struct{
    int priority;
    int timequantum;
    char *processes[num_queues][max_processes];
}MLFQ;

void add_process( char process[] ){
    for (int i=0; i<max_processes; i++){
        if (MLFQ.processes[0][i]==NULL){
            MLFQ.processes[0][i]=process;
            break;
        }

}
}

int schedule(int *queue, int *column){
    for (int i=0; i<num_queues; i++){
        for (int j=0;j<max_processes;j++){
            if (MLFQ.processes[i][j]!=NULL){
                *queue = i;
                *column=j;
                return 1;
            }
        }
    }
    return 0;
} 

int main(){
    add_process("SIGN_IN");
    add_process("Open_start");
    add_process("open_firefox");
    int i, j;


    for (j=0; j<MLFQ.processes; j++){
        if  (schedule(&i,&j)){
            printf("%s was selected. \n", MLFQ.processes[i][j]);
            MLFQ.processes[i][j]=NULL;
        }

        else return 0;
    
}}
