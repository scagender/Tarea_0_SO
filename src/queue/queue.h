#pragma once
#include "../process/process.h"
#ifndef _QUEUE_H
#define _QUEUE_H
typedef int item_type;
typedef struct _queue* queue;
queue queue_create();
void queue_destroy(queue q);
void queue_clear(queue q);
void queue_push(queue q, Process* elem);
int queue_size(queue q);
Process* queue_pop(queue q);
Process* queue_pop_ready(queue q);
void revisar_queue(queue q, double tiempo_actual);
#endif