#pragma once
#include "../process/process.h"

typedef struct Queue* Queue;

Queue* crear_lista();
void agregar_proceso(Queue* lista, Process proceso);
Process eliminar_proceso(Queue* lista);
int largo_de_lista(Queue* lista);
void queue_clear(Queue* lista);

