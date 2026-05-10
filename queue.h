#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct {
    int *data;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

Queue* create_queue(int capacity);
bool is_empty(Queue *q);
void enqueue(Queue *q, int item);
int dequeue(Queue *q);
void free_queue(Queue *q);

#endif // QUEUE_H