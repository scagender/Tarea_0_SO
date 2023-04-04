#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <signal.h>
#include<sys/wait.h>
#include<time.h>

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
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
				entrante = queue_pop_ready(cola, (double)clock());			// Saca un NULL cuando todos estan en WAIT
				if (entrante != NULL){
					listo1 = 1;
					break;
				}
			}
			printf("Empiezo el proceso\n");
			entrante->estado = "RUNNING";
			entrante->wtime = entrante->wtime + clock();
			entrante -> entradas = entrante -> entradas + 1;
			if(entrante->entradas == 1)
			{
				entrante->rtime = clock();
			}
			printf("%s\n", entrante->nombre);
			printf("%d\n", entrante->entradas);
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
								ultima_entrada = (clock()-tiempo_inicio)/CLOCKS_PER_SEC;
							}
						}
						if ((double)(clock()-time)/CLOCKS_PER_SEC>burst)
						{
							kill(pid2, SIGSTOP);
							printf("El proceso hijo ha terminado debido a una señal\n");
							entrante->estado = "WAITING";
							entrante->inicio_w = clock();
							queue_push(cola, entrante);//DEVUELVO A LA COLA
							printf("Se fue a la cola");
							break;

						}
					
						if (waitpid(pid2, &status, WNOHANG)>0)
							{ // El proceso hijo terminó debido a una señal o un error
     	 					printf("El proceso hijo termino\n");
							procesos++;
							break;
							}
					}
			}
			if (entrante-> entradas ==1)
			{
				int hpid = fork();
				if (hpid == 0) 
				{//proceso hijo
					if(entrante -> entradas == 1)
					{
						int largo = atoi(input_file->lines[entrante->pid][5]);
						char *args[] = {NULL};
						for(int contador = 0; contador < largo; ++contador)
						{
							args[contador] = input_file->lines[entrante -> pid][6+contador];
						}
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
     	 					printf("El proceso hijo termino\n");
							procesos++;
							break;
							}
        			}
			}
		}
		}	
		if(procesos==input_file->len)
		{
			a++;
		}
	}
	char *file_out = (char *)argv[2];
	FILE* fichero; 
	fichero= fopen(file_out,"w");
	fputs("aprende a prgramar \n",fichero);
	
	input_file_destroy(input_file);
 	
 	return 0;
	
}