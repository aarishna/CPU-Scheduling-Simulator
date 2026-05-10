#include <stdlib.h>
#include "queue.h"

Queue* create_queue(int capacity) {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->capacity = capacity;
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->data = (int*)malloc(capacity * sizeof(int));
    return q;
}

bool is_empty(Queue *q) {
    return q->size == 0;
}

void enqueue(Queue *q, int item) {
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = item;
    q->size++;
}

int dequeue(Queue *q) {
    int item = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return item;
}

void free_queue(Queue *q) {
    free(q->data);
    free(q);
}