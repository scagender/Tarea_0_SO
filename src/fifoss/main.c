#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <time.h>
#include <signal.h>
#include <string.h>


void handler(int sig) {					//Parar proceso
    pid_t pid = getpid(); //Padre
    printf("Se recibió la señal SIGALRM en el proceso %d, por lo que se va a parar el proceso.\n", pid);
    kill(pid, SIGSTOP);
}

void handler2(int sig) {				//Reanudar Proceso
    pid_t pid = getpid();
    printf("Se recibió la señal SIGCOUNT para el proceso %d, por lo que se va a reanudar su ejecución.\n", pid);
    //kill(pid, SIGCONT);			//Con esto se sigue la ejecucción
}

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	clock_t tiempo_inicio = clock();
	int a= 0;
	int procesos=0;
	int menor=0;
	queue cola;
	cola = queue_create();
	while(a==0)
	{
		clock_t t = clock();
		double tt = t - tiempo_inicio;
		double tta = tt / CLOCKS_PER_SEC;
		for (int i = 0; i < input_file->len; ++i)
		{
			if(atoi(input_file->lines[i][1]) == menor)
			{
				Process* proceso= process_init(input_file->lines[i][0], i, "READY", atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), clock(),0,0,0);
				queue_push(cola, proceso);
			}
		}
		
		if (queue_size(cola)>0)
		{
			Process* entrante = queue_pop(cola);
			entrante->estado = "RUNNING";
			entrante->wtime = entrante->wtime + clock();
			entrante -> entradas += 1;
			printf("\n\nNúmero de entradas: %i\n", entrante -> entradas);
			if(entrante->entradas == 1)
			{
				entrante->rtime = clock();
			}
			printf("%s\n", entrante->nombre);
			pid_t Hid;
			if (entrante -> entradas == 1){
				Hid = fork();
				entrante-> Hid = Hid;}//Ocupar pid es reescribir el del padre
									//Si ya entra por segunda vez se hace otro proceso hijo, y al reanudarlo empiezan 2 procesos
			else{Hid = entrante -> Hid;}
			printf("Soy el proceso con Hid: %d\n", Hid);
			if (Hid == -1) {
				perror("Error en fork()");
				exit(1);
			} else if (Hid == 0) {				//Hijo
				signal(SIGALRM, handler);
				signal(SIGCONT, handler2);

				if(entrante -> entradas == 1){
					int largo = atoi(input_file->lines[entrante->pid][5]);
					char *args[] = {input_file->lines[entrante -> pid][4]};
					if (largo > 0){
						for(int contador = 0; contador <= largo; ++contador){
						args[contador+1] = input_file->lines[entrante -> pid][6+contador];
						
					}}
					args[largo+1] = NULL;
					printf("Voy a ejecutar\n");
					//execv(args[0], args);
					sleep(10);
					}						//exit (0); wait(retval)
				printf("Esto tiempo va a correr antes de esperar: %f\n", entrante->burst);
				//signal(SIGALRM, handler);	//Ahora si para el hijo, Esta es la conexion entre alarma y quien la maneja
				//alarm(entrante->burst);							//Al terminar envia una alarma de tipo SIGALARM

				}
			else { 							// Este es el proceso padre
			//wait(&status) == sleep; WEXISTATUS(status) es pa leer el tipo de dato q tira wait
				int status;
				if(entrante -> entradas > 1){
					printf("Envio la señal para reanudar\n");
					kill(entrante->Hid, SIGCONT);}
				for(int dormir = 0; dormir <= entrante->burst; ++dormir){
					sleep(1);
				}
				//waitpid(Hid, &status, WNOHANG);							
				if (WIFEXITED(status)) {
					printf("El proceso hijo terminó normalmente.\n");
					entrante->estado = "FINISHED";
					++procesos;
					entrante->Hid = -1;
					printf("Se cancela la alarma.\n");
				} else {
					alarm(0);
					printf("El proceso %i fue interrumpido por la señal %d.\n", entrante->Hid, WTERMSIG(status));
					entrante->estado = "WAITING";
					printf("Ahora me voy al final de la cola\n");
					queue_push(cola, entrante);
					//printf("Voy a mimir: %f\n", entrante->wait);
					//sleep(entrante->wait);
				}	
				
			}
				}
		if(procesos==input_file->len)
		{
			a++;
		}
		
	menor++;
	}
	char *file_out = (char *)argv[2];
	FILE* fichero; 
	fichero= fopen(file_out,"w");
	fputs("aprende a prgramar \n",fichero);
	
	input_file_destroy(input_file);
 	
 	return 0;
	
}																//./fifoss example.txt out.csv
																// ./fifoss ./testsT0/inputs/test3.txt output.csv