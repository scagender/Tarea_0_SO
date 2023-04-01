#include "process.h"
#include <stdlib.h> // malloc, calloc, free, etc
Process* process_init(char* nombre,int pid,char* estado,double burst,double wait, double inicio)
{
  Process* process = malloc(sizeof(Process));

  // Lo inicializamos
  *process = (Process){
    .nombre = nombre,
    .pid = pid,
    .estado = estado,
    .burst = burst,
    .wait = wait,
    .inicio = inicio
  };

  // Lo retornamos
  return process;
}



void process_destroy(Process *process)
{
    free(process);
}