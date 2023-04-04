
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "../process/process.h"
#include <string.h>
#include<time.h>
//https://d-michail.github.io/assets/teaching/data-structures/018_QueueImplementation.en.pdf
//cola fifo sacada de ese link con algunos cambios para que acepte procesos.
struct node {
struct Process* data;
struct node* next;
};
struct _queue {             //./fifoss ./testsT0/inputs/test1.txt output.csv
struct node* head;
struct node* tail;
int size;
};
queue queue_create() {
queue q = (queue) malloc(sizeof(struct _queue));
if (q == NULL) {
fprintf(stderr, "Insufficient memory to \
initialize queue.\n");
abort();
}
q->head = NULL;
q->tail = NULL;
q->size = 0;
return q;
}

void queue_destroy(queue q) {
if (q == NULL) {
fprintf(stderr, "Cannot destroy queue\n");
abort();
}
queue_clear(q);
free(q);
}
void queue_clear(queue q) {
if (q==NULL) {
fprintf(stderr, "Cannot work with NULL queue.\n");
abort();
}
while(q->head != NULL) {
struct node* tmp = q->head;
q->head = q->head->next;
free(tmp);
}
q->tail = NULL;
q->size = 0;
}

void queue_push(queue q, Process* elem) {
struct node* n;
n = (struct node*) malloc(sizeof(struct node));
if (n == NULL) {
fprintf(stderr, "Insufficient memory to \
create node.\n");
abort();
}
n->data = elem;
n->next = NULL;
if (q->head == NULL) {
q->head = q->tail = n;
} else {
q->tail->next = n;
q->tail = n;
}
q->size += 1;
}
int queue_size(queue q) {
if (q==NULL) {
fprintf(stderr, "Cannot work with NULL queue.\n");
abort();
}
return q->size;
}

Process* queue_pop_ready(queue q, double tiempo_actual) {
    struct node* nodo = q->head;
    struct node* prev = NULL;
    while (nodo != NULL) {
        revisar_nodo(tiempo_actual, nodo->data);
        if (strcmp(nodo->data->estado, "READY") == 0) {
            // el nodo actual tiene estado "READY"
            if (prev == NULL) {
                // el nodo actual es el primero de la lista
                q->head = nodo->next;
            } else {
                // el nodo actual no es el primero de la lista
                prev->next = nodo->next;
            }
            return nodo->data; // devolvemos el nodo actual
        }
        //Buscamos en el siguiente nodo
        prev = nodo;
        nodo = nodo->next;
    }

  // no se encontró ningún nodo con estado "READY"
  return NULL;
}

Process* queue_pop(queue q) {
    struct node* head = q->head;
    if (q->head == q->tail) {
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head = q->head->next;
    }
    q->size -= 1;
    Process* data;
    data = head->data;
    free(head);
    return data;
    }


void revisar_nodo(double tiempo_actual, Process* revisado){
    if (strcmp(revisado->estado, "WAITING") == 0){
        double tiempo_esperando = (double)(tiempo_actual-revisado->inicio_w)/CLOCKS_PER_SEC;
        if (tiempo_esperando >= revisado->wait)
        {
            revisado-> estado = "READY";
        }
    }
}
    