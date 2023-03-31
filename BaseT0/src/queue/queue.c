#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "../process/process.h"


typedef struct Nodo {           //https://d-michail.github.io/assets/teaching/data-structures/018_QueueImplementation.en.pdf
    Process proceso;            //https://www.geeksforgeeks.org/queue-linked-list-implementation/
    struct Nodo* siguiente;     //https://www.infocodify.com/c-programming/queue
} Nodo;


typedef struct queue
{
    int largo;
    Nodo* primero;
    Nodo* ultimo;
    //Hacerlo con nodos
    //struct queue* next; Para listas ligadas

} Queue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Inicialización de la cola
Queue* crear_lista() {
    Queue* lista = malloc(sizeof(Queue));
    if (lista == NULL) {
        fprintf(stderr, "No hay memoria suficiente para crear la cola.\n");
        abort();
    }
    lista -> largo = 0;
    lista -> primero = NULL;
    lista -> ultimo = NULL;
    return lista;
}



// Función para crear un nodo de la cola
Nodo* crear_nodo(Process proceso) {
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    if (nodo == NULL) {
        fprintf(stderr, "No hay memoria suficiente para crear el nodo.\n");
        abort();
    }
    nodo -> proceso = proceso;
    nodo -> siguiente = NULL;
    return nodo;
};


// Función para agregar un proceso a la cola (Push)
void agregar_proceso(Queue* lista, Process proceso) {
    Nodo* nodo = crear_nodo(proceso);
    if (lista -> primero == NULL) {
        // Si la cola está vacía, el nodo nuevo será el primero y el último
        lista -> primero = nodo;
        lista -> ultimo = nodo;
    } else {
        // Si la cola ya tiene elementos, se agrega el nodo nuevo al final
        lista -> ultimo -> siguiente = nodo;
        lista -> ultimo = nodo;
        lista -> largo += 1;
    }
}

//Eliminar proceso (pop python)
Process eliminar_proceso(Queue* lista) {
    Nodo* nodo_eliminado;
    if (lista -> primero == NULL) {
        // Si la cola está vacía, se retorna un proceso con id -1
        lista -> primero = NULL;
        lista -> ultimo = NULL;
    } else {
        // Se guarda el nodo que se va a eliminar para liberar memoria
        nodo_eliminado = lista -> primero;
        // Se actualiza el primer nodo de la cola
        lista -> primero = lista -> primero -> siguiente;
        // Si la cola quedó vacía, se actualiza el último nodo
        if (lista -> primero == NULL) {
            lista -> ultimo = NULL;
        }
        // Se libera el nodo eliminado de la memoria
        free(nodo_eliminado);
        lista -> largo -= 1;
        // Se retorna el proceso eliminado
        return nodo_eliminado -> proceso;
    }
}

int largo_de_lista(Queue* lista) {
    if (lista == NULL) {
        fprintf(stderr, "La lista es NULL.\n");
        abort();
    }
    return lista -> largo;
}

void queue_clear(Queue* lista) {
    if (lista == NULL) {
    fprintf(stderr, "La lista es NULL.\n");
    abort();
    }
    while(lista -> primero != NULL) {
        free(lista -> primero);
        lista -> primero = lista -> primero -> siguiente;  
    }
    //aseguramos que el último sea null
    lista -> ultimo = NULL;
    lista -> largo = 0;
}

int main(){
    return 0;
}
