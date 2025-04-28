#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int fdUt1[2];
    int fdUt2[2];
    int fdUt3[2];
    int fdUt4[2];
    int fdUt5[2];
    
    printf("Accensione di Discord (attesa di 10 secondi)...\n");
    sleep(10);
    printf("Discord è attivo.");
    
    
}