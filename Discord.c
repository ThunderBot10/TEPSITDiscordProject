#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct Messaggio
{
    char *mitt[4];
    char *mess[4096];
};

void stampaErrore(int tipo);

int main(int argc, char *argv[])
{
    /* directory delle named pipe tra gli utenti */
    char *pipes[5] = {"/tmpPipe/fifo1", "/tmpPipe/fifo2", "/tmpPipe/fifo3", "/tmpPipe/fifo4", "/tmpPipe/fifo5"};
    
    /* named pipe per gli utenti */
    int fdut[5];

    int pid; /* pid necessario per la fork */
    int fd[2]; /* unamed pipe per il processo */

    if (mkfifo(pipe1, 0666) == -1){
        stampaErrore(1);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 5; i++)
    {
        pid = fork(); /* creazione del processo figlio */
        /* controllo di possibili errori nella esecuzione della fork */
        if(pid == -1)
        {
            /* chiusura del programma in caso di errore */
            stampaErrore(2);
            exit(EXIT_FAILURE);
        }

        /* processo figlio che andrà ad occuparsi della ricezione ed attesa di un messaggio proveniente dall'esterno */
        if(pid == 0)
        {
            /* apertura delle named pipe */
            fdut[i] = open(pipes[i], O_RDONLY);

            if(fdut[i] == -1)
            {
                stampaErrore(3);
                exit(EXIT_FAILURE);
            }

            char *buffer[4096];

            buffer = read(fdut[i], *buffer, sizeof(buffer));
            while(!(strcmp(buffer, "/exit")))
            {

            }
        }
    }
    

}

void stampaErrore(int tipo)
{
    perror("Errore nell'esecuzione del programma.");
    switch (tipo)
    {
        case 1:
            perror("Errore: non è stata creata correttamente la named pipe.");
            break;
        
        case 2:
            perror("Errore: impossibile creare il processo figlio.");
            break;

        case 3:
            perror("Errore: impossibile aprire una o più named pipe.");
            break;

        default:
            break;
    }

    printf("Chiusura del programma...");
    return;
}