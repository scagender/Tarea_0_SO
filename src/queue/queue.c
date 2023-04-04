
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

Process* queue_pop_ready(queue q) {
    struct node* nodo = q->head;
    struct node* prev = NULL;
    printf("Estado: %s\n", nodo->data->estado);
    while (nodo != NULL) {
        if (strcmp(nodo->data->estado, "READY") == 0) {
            // el nodo actual tiene estado "READY"
            if (prev == NULL) {
                // el nodo actual es el primero de la lista
                q->head = nodo->next;
            } else {
                // el nodo actual no es el primero de la lista
                prev->next = nodo->next;
            }
            printf("Retorno algo diferente a NULL\n");
            return nodo->data; // devolvemos el nodo actual
        }
        //Buscamos en el siguiente nodo
        prev = nodo;
        nodo = nodo->next;
    }

  // no se encontró ningún nodo con estado "READY"
  printf("Retorno NULL\n");
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


void revisar_queue(queue cola, double tiempo_actual){
    struct node* nodo_revisado = cola->head;
    while (nodo_revisado){
        Process* revisado = nodo_revisado->data;
        if (revisado->estado == "WAITING"){
            printf("Esta waiting\n");
            double tiempo_esperando = (double)(tiempo_actual-revisado->inicio_w)/CLOCKS_PER_SEC;
            printf("El tiempo esta bien: %f\n", tiempo_esperando);
            if (tiempo_esperando >= revisado->wait)
            {
                printf("Ahora Esta ready\n");
                revisado-> estado = "READY";
            }
            else{printf("No esta ready todavia\n");}
        }
        else
        {
            printf("Esta ready\n");
        }
        nodo_revisado = nodo_revisado->next;
        }

}
    