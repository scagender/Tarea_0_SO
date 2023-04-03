#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv){
    double begin = clock();
    printf("CHILD: Iniciado...\n");
    while((double)(clock() - begin) / CLOCKS_PER_SEC < 10);
    return 0;
}