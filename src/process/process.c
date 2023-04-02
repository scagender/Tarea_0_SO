#include "process.h"
#include <unistd.h>
#include <stdlib.h> // malloc, calloc, free, etc
Process* process_init(char* nombre,int pid,char* estado,double burst,double wait, double inicio, double ttime, double rtime, double wtime, int entradas)
{
  Process* process = malloc(sizeof(Process));

  // Lo inicializamos
  *process = (Process){
    .nombre = nombre,
    .pid = pid,
    .estado = estado,
    .burst = burst,
    .wait = wait,
    .inicio = inicio,
    .ttime = ttime,
    .rtime = rtime,
    .wtime = wtime,
    .entradas = entradas,
    .Hid = -1             //Pid del Hijo para saber cual es su hijo cuando se quiera reanudar el proceso
  };

  // Lo retornamos
  return process;
}



void process_destroy(Process *process)
{
    free(process);
}