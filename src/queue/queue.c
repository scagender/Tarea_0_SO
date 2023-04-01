
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "../process/process.h"

struct node {
struct Process* data;
struct node* next;
};
struct _queue {
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
queue_pop(queue q) {
struct node* head = q->head;
if (q->head == q->tail) {
q->head = NULL;
q->tail = NULL;
} else {
q->head = q->head->next;
}
q->size -= 1;
Process* data = head->data;
free(head);
return data;
}