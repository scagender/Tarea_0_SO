#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char **argv){
    double begin = clock();
    printf("CHILDFAULTER: Iniciado...\n");
    while((double)(clock() - begin) / CLOCKS_PER_SEC < 15);
    return 1;
}