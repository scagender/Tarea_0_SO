#pragma once
// Define the struct
typedef struct Process {
  char nombre;
  int pid;
  char estado;
  double burst;
  double wait;
  double inicio;
} Process;

// Declare functions
void process_destroy(Process *process);