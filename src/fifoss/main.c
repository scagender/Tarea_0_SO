#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <time.h>
#include <signal.h>


void handler(int sig, pid_t Hid) {
    printf("Se recibió la señal SIGALRM.\n");
    kill(Hid, SIGSTOP);
    if (kill(Hid, 0) == 0) { // El proceso hijo sigue vivo
        printf("El proceso hijo con PID %d sigue vivo. Enviando señal SIGCONT.\n", Hid);
        kill(Hid, SIGCONT);
    } else { // El proceso hijo ya terminó
        printf("El proceso hijo con PID %d ya terminó.\n", Hid);
    }
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
		clock_t t =clock();
		double tt = t - tiempo_inicio;
		double tta = tt / CLOCKS_PER_SEC;
		for (int i = 0; i < input_file->len; ++i)
		{
			if(atoi(input_file->lines[i][1]) == menor)
			{
				Process* proceso= process_init(input_file->lines[i][0], i, "READY", atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), clock(),0,0,0,0);
				queue_push(cola, proceso);
			}
		}
		if (queue_size(cola)>0)
		{
			Process* entrante = queue_pop(cola);
			entrante->estado = "RUNNING";
			entrante->wtime = entrante->wtime + clock();
			entrante -> entradas = entrante -> entradas + 1;
			if(entrante->entradas == 1)
			{
				entrante->rtime = clock();
			}
			printf("%s\n", entrante->nombre);
				++procesos;
			pid_t Hid = fork();					//Ocupar pid es reescribir el del padre
			if (Hid == -1) {
				perror("Error en fork()");
				exit(1);
			} else if (Hid == 0) {
				if(entrante -> entradas == 1){
					int largo = atoi(input_file->lines[entrante->pid][5]);
					char *args[] = {NULL};
					for(int contador = 0; contador < largo; ++contador){
						args[contador] = input_file->lines[entrante -> pid][6+contador];
					}
					execv(input_file->lines[entrante -> pid][4], args);
					}
				exit(0);
				}
				else { // Este es el proceso padre
				int status;
				waitpid(Hid, &status, WUNTRACED); // Esperar a que el proceso hijo termine o sea detenido, esto hay que ocupar según el enunciado
				if (WIFSTOPPED(status)) {
					printf("El proceso hijo con PID %d ha sido detenido.\n", Hid);
					entrante->Hid = -1; // Establecer pid en -1 para indicar que el proceso no tiene un hijo en ejecución
				} else {
					printf("El proceso hijo con PID %d terminó con el estado %d.\n", Hid, status);
					entrante->estado = "FINISHED";
					entrante->Hid = -1; // Establecer pid en -1 para indicar que el proceso no tiene un hijo en ejecución
    }
		}
		}		
			
													// Este es el proceso hijo
													//signal(SIGALRM, handler);
													//alarm(entrante -> burst); // Establecer una alarma para enviar SIGALRM después de 5 segundos
													//while (1) {
														// Haz algo en el proceso hijo aquí
													//}	
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
	
}