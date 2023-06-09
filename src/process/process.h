#pragma once
// Define the struct
typedef struct Process {
  char* nombre;
  int pid;
  char* estado;
  double burst;
  double wait;
  double inicio;
  double ttime; 
  double rtime; 
  double wtime;
  int entradas;
  double inicio_w;
} Process;

// Declare functions
Process* process_init(char* nombre,int pid,char* estado,double burst,double wait, double inicio, double ttime, double rtime, double wtime, int entradas, double inicio_w);
void process_destroy(Process *process);

