#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <signal.h>
#include<sys/wait.h>
#include<time.h>

int main(int argc, char const *argv[])						//Arreglar el childmine, deben llegar al mismo tiempo en test4
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	char *file_out = (char*)argv[2];
    FILE* fichero;
    fichero = fopen(file_out,"wt");
	int a= 0;
	int procesos=0;
	queue cola;
	cola = queue_create();
	double tiempo_inicio = clock();
	double ultima_entrada= 0;
	while(a==0)
	{	
		for (int i = 0; i < input_file->len; ++i)
		{
			if(atoi(input_file->lines[i][1]) >= ultima_entrada && atoi(input_file->lines[i][1]) <= (clock()-tiempo_inicio)/CLOCKS_PER_SEC)
			{
				Process* proceso= process_init(input_file->lines[i][0], i, "READY", atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), clock(),0,0,0,0,0);
				queue_push(cola, proceso);
				ultima_entrada = (clock()-tiempo_inicio)/CLOCKS_PER_SEC;
			}
		}
		if (queue_size(cola)>0)
		{
			int listo1 = 0;
			Process* entrante;
			while(listo1 == 0){
				for (int i = 0; i < input_file->len; ++i)				//revisa si llego otro proceso
				{
					if(atoi(input_file->lines[i][1]) >= ultima_entrada && atoi(input_file->lines[i][1]) <= (clock()-tiempo_inicio)/CLOCKS_PER_SEC)
					{
						Process* proceso= process_init(input_file->lines[i][0], i, "READY", atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), clock(),0,0,0,0,0);
						queue_push(cola, proceso);
						ultima_entrada = (clock()-tiempo_inicio)/CLOCKS_PER_SEC;
					}
				}
				entrante = queue_pop_ready(cola, (double)clock());			// Saca un NULL cuando todos estan en WAIT
				if (entrante != NULL){
					listo1 = 1;
					break;
				}
			}
			double tiempo_wait = (clock()-entrante->inicio_w)/CLOCKS_PER_SEC;
			entrante->wtime = entrante->wtime + tiempo_wait;
			entrante->estado = "RUNNING";
			entrante -> entradas = entrante -> entradas + 1;
			if(entrante->entradas == 1)
			{
				entrante->rtime = (clock()-entrante->inicio)/CLOCKS_PER_SEC;
			}
			printf("%s\n", entrante->nombre);
			printf("Entradas %d\n", entrante->entradas);
			int entradas= entrante->entradas;
			if (entrante -> entradas > 1)
			{
				int pid2= entrante->pid;
				int status;
				int burst = entrante->burst;
				kill(pid2 , SIGCONT);
				double time = clock();
				while(1)
					{
						for (int i = 0; i < input_file->len; ++i)
						{
							if(atoi(input_file->lines[i][1]) >= ultima_entrada && atoi(input_file->lines[i][1]) <= (clock()-tiempo_inicio)/CLOCKS_PER_SEC)
							{
								Process* proceso= process_init(input_file->lines[i][0], i, "READY", atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), clock(),0,0,0,0,0);
								queue_push(cola, proceso);
								ultima_entrada = (double)(clock()-tiempo_inicio)/CLOCKS_PER_SEC;
							}
						}
						if ((double)(clock()-time)/CLOCKS_PER_SEC>burst)
						{
							kill(pid2, SIGSTOP);
							printf("El proceso hijo ha terminado debido a una señal\n");
							entrante->estado = "WAITING";
							entrante->inicio_w = clock();
							queue_push(cola, entrante);//DEVUELVO A LA COLA
							break;

						}
					
						if (waitpid(pid2, &status, WNOHANG)>0)
							{ // El proceso hijo terminó debido a una señal o un error
     	 					printf("El proceso hijo termino\n");
							double turn_time = (clock()-entrante->inicio)/CLOCKS_PER_SEC;
                            int exit_code = WEXITSTATUS(status);
                            fprintf(fichero, "%s,%i,%i,%f,%f,%f,%i\n", entrante->nombre, entrante->pid, entrante->entradas,turn_time,entrante->rtime,entrante->wtime,exit_code);
							procesos++;
							process_destroy(entrante);
							break;
							}
					}
			}
			else if (entrante-> entradas == 1)
			{
				int hpid = fork();
				if (hpid == 0) 
				{//proceso hijo
					if(entrante -> entradas == 1)
					{
						int largo = atoi(input_file->lines[entrante->pid][5]);
						char *args[] = {input_file->lines[entrante -> pid][4]};
						int paso = 0;
						for(int contador = 0; contador < largo; ++contador)
						{
							args[contador+1] = input_file->lines[entrante -> pid][6+contador];
							paso = 1;
							if (paso == 1 && largo-contador ==1){
								args[largo+1] = NULL;
							}
						}
						if (paso == 0){
							args[1] = NULL;
						}
						printf("Empieza la ejecucción\n");
						execvp(input_file->lines[entrante -> pid][4], args);
					}
				}
				
				else{
					double time2= clock();
					int status; 
					int burst = entrante->burst;
					entrante->pid= hpid;
					while(1)
					{
						for (int i = 0; i < input_file->len; ++i)
						{
							if(atoi(input_file->lines[i][1]) >= ultima_entrada && atoi(input_file->lines[i][1]) <= (clock()-tiempo_inicio)/CLOCKS_PER_SEC)
							{
								Process* proceso= process_init(input_file->lines[i][0], i, "READY", atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), clock(),0,0,0,0,0);
								queue_push(cola, proceso);
								ultima_entrada = (clock()-tiempo_inicio)/CLOCKS_PER_SEC;
							}
						}
						if ((double)(clock()-time2)/CLOCKS_PER_SEC>burst)
						{
							kill(hpid, SIGSTOP);
							printf("El proceso hijo ha terminado debido a una señal o un error\n");
							entrante->estado = "WAITING";
							entrante->inicio_w = clock();
							queue_push(cola, entrante);//DEVUELVO A LA COLA
							break;		
						}
						if (waitpid(hpid, &status, WNOHANG)>0)
							{ // El proceso hijo terminó debido a una señal o un error
     	 					printf("El proceso hijo termino\n");			// Eliminar proceso y cola
							double turn_time = (double)(clock()-entrante->inicio)/CLOCKS_PER_SEC;
                            int exit_code = WEXITSTATUS(status);
                            fprintf(fichero, "%s,%i,%i,%f,%f,%f,%i\n", entrante->nombre, entrante->pid, entrante->entradas,turn_time,entrante->rtime,entrante->wtime,exit_code);
							procesos++;
							process_destroy(entrante);
							break;
							}
        			}
			}
		}
		}	
		if(procesos==input_file->len)
		{
			a++;
			queue_destroy(cola);
		}
	}

	input_file_destroy(input_file);
	fclose(fichero);
 	
 	return 0;
	
}