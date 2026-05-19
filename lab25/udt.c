#include <stdio.h>
#include "udt.h"

void queue_create(queue *q) {
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

bool queue_is_empty(const queue *q) {
    return q->size == 0;
}

size_t queue_size(const queue *q) {
    return q->size;
}

void queue_push_back(queue *q, data_type item) {
    if (q->size >= MAX_SIZE) return;
    q->data[q->tail] = item;
    q->tail = (q->tail + 1) % MAX_SIZE;
    q->size++;
}

data_type queue_pop_front(queue *q) {
    data_type empty = {0};
    if (queue_is_empty(q)) return empty;
    
    data_type item = q->data[q->head];
    q->head = (q->head + 1) % MAX_SIZE;
    q->size--;
    return item;
}

void queue_print(const queue *q) {
    if (queue_is_empty(q)) {
        printf("Empty\n");
        return;
    }
    int idx = q->head;
    for (size_t i = 0; i < q->size; i++) {
        printf("[%d: %s] ", q->data[idx].key, q->data[idx].value.symbols);
        idx = (idx + 1) % MAX_SIZE;
    }
    printf("\n");
}

void queue_concat(queue *q1, const queue *q2) {
    int idx = q2->head;
    for (size_t i = 0; i < q2->size; i++) {
        queue_push_back(q1, q2->data[idx]);
        idx = (idx + 1) % MAX_SIZE;
    }
}
