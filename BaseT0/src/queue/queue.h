#pragma once
#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "../process/process.h"

typedef struct _Queue* Queue;
void agregar_proceso(Queue lista, Process* proceso);
Process* eliminar_proceso(Queue lista);
int largo_de_lista(Queue lista);
void queue_clear(Queue lista);
Queue crear_lista();

#endif