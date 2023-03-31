#include "process.h"
Process* process_init(char nombre,int pid,char estado,double burst,double wait, double inicio)
{
  // Pedimos memoria para Reptil
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