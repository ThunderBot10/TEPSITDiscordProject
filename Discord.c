#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void stampaErrore(int tipo);

int main(int argc, char *argv[])
{
    /* directory delle named pipe tra gli utenti */
    char pipe1[20] = "/tmpPipe/fifo1";
    char pipe2[20] = "/tmpPipe/fifo2";
    char pipe3[20] = "/tmpPipe/fifo3";
    char pipe4[20] = "/tmpPipe/fifo4";
    char pipe5[20] = "/tmpPipe/fifo5";
    
    /* named pipe per gli utenti */
    int fd1;
    int fd2;
    int fd3;
    int fd4;
    int fd5;

    int pid; /* pid necessario per la fork */
    int fd[2]; /* unamed pipe per il processo */

    /* creazione della unnamed pipe tra i processi padre e figlio e controllo della corretta creazione */
    if(pipe(fd) == -1)
    {
        /* chiusura del programma in caso di errore */
        stampaErrore(1);
        exit(EXIT_FAILURE);
    }

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
        fd1 = open(pipe1, O_RDONLY);
        fd2 = open(pipe2, O_RDONLY);
        fd3 = open(pipe3, O_RDONLY);
        fd4 = open(pipe4, O_RDONLY);
        fd5 = open(pipe5, O_RDONLY);

        if((fd1 == -1) || (fd2 == -1) || (fd3 == -1) || (fd4 == -1) || (fd5 == -1))
        {
            stampaErrore(3);
            exit(EXIT_FAILURE);
        }

        
    }
    else
    /* processo padre che andrà ad occuparsi della trasmissione di un messaggio verso l'esterno */
    {

    }

}

void stampaErrore(int tipo)
{
    perror("Errore nell'esecuzione del programma.");
    switch (tipo)
    {
        case 1:
            perror("Errore: non è stata creata correttamente la unamed pipe.");
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