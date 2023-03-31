#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <time.h>

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	int A[input_file->len];
	clock_t tiempo_inicio = clock();
	int a= 0;
	int procesos=0;
	int menor=0;
	while(a==0)
	{
		clock_t t =clock();
		double tt = t - tiempo_inicio;
		double tta = tt / CLOCKS_PER_SEC;
		for (int i = 0; i < input_file->len; ++i)
		{
			if(atoi(input_file->lines[i][1]) == menor)
			{
				printf("%s ", input_file->lines[i][0]);
				printf("\n");
				Process* process= process_init(input_file->lines[i][0], i, "READY", )
				++procesos;
			}

			if(procesos>=input_file->len)
			{
				a++;
			}
		}
		++menor;
	}
	char *file_out = (char *)argv[2];
	FILE* fichero; 
	fichero= fopen(file_out,"w");
	fputs("aprende a prgramar \n",fichero);
	
	
 	
 	return 0;
	
	input_file_destroy(input_file);
}