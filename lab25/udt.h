#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_SIZE 100

typedef int key_type;

typedef struct {
    char symbols[8];
} value_type;

typedef struct {
    key_type key;
    value_type value;
} data_type;

// Очередь
typedef struct {
    data_type data[MAX_SIZE];
    int head;
    int tail;
    size_t size;
} queue;

void queue_create(queue *q);
bool queue_is_empty(const queue *q);
void queue_push_back(queue *q, data_type item);
data_type queue_pop_front(queue *q);
void queue_print(const queue *q);
size_t queue_size(const queue *q);

void queue_concat(queue *q1, const queue *q2);

#endif
