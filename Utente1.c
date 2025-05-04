#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

struct Messaggio
{
    char *mitt[4];
    char *mess[4096];
};

void stampaErrore(int tipo);

int main(int argc, char *argv[])
{
    const char *pipe1 = "/tmpPipe/fifo1";
    int fd;

    if (mkfifo(pipe1, 0666) == -1)
    {
        stampaErrore(1);
        exit(EXIT_FAILURE);
    }

    int pid = fork();
    if (pid == -1)
    {
        stampaErrore(2);
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        fd = open(pipe1, O_RDONLY);
        if (fd == -1)
        {
            stampaErrore(3);
            exit(EXIT_FAILURE);
        }

        char buffer[256];
        while (read(fd, buffer, sizeof(buffer)) > 0 || !(strcmp(buffer, "/exit")))
        {
            printf("Processo figlio ha ricevuto: %s\n", buffer);
        }

        close(fd);
    } else {
        // Parent process: Writes to the named pipe
        fd = open(pipe1, O_WRONLY);
        if (fd == -1) {
            stampaErrore(3);
            exit(EXIT_FAILURE);
        }

        char *message = "Ciao dal processo utente";
        write(fd, message, strlen(message) + 1);
        printf("Processo padre ha inviato: %s\n", message);

        close(fd);
    }

    return 0;
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
            perror("Errore: impossibile aprire la named pipe in scrittura o in lettura.");
            break;

        default:
            break;
    }

    printf("Chiusura del programma...");
    return;
}