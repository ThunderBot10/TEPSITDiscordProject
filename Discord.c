#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

struct Messaggio
{
    char mitt[4];
    char mess[4096];
};

void stampaErrore(int tipo);

int main(int argc, char *argv[])
{
    /* directory delle named pipe tra gli utenti */
    char *pipes[5] = {"/tmpPipe/fifo1", "/tmpPipe/fifo2", "/tmpPipe/fifo3", "/tmpPipe/fifo4", "/tmpPipe/fifo5"};
    
    /* named pipe per gli utenti */
    int fdut[5];

    mkdir("/tmpPipe", 0777); // Ensure the directory exists
    for (int i = 0; i < 5; i++)
    {
        if (mkfifo(pipes[i], 0666) == -1)
        {
            stampaErrore(1);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int pid = fork(); /* creazione del processo figlio */
        if (pid == -1)
        {
            stampaErrore(2);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            fdut[i] = open(pipes[i], O_RDONLY);
            if (fdut[i] == -1)
            {
                stampaErrore(3);
                exit(EXIT_FAILURE);
            }

            struct Messaggio messaggio;
            ssize_t bytes_read;
            while ((bytes_read = read(fdut[i], messaggio.mess, sizeof(messaggio.mess) - 1)) > 0)
            {
                messaggio.mess[bytes_read] = '\0';
                if (strcmp(messaggio.mess, "/exit") == 0)
                {
                    break;
                }
                printf("Messaggio ricevuto da %s: %s\n", messaggio.mitt, messaggio.mess);
            }

            close(fdut[i]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            fdut[i] = open(pipes[i], O_WRONLY);
            if (fdut[i] == -1)
            {
                stampaErrore(3);
                exit(EXIT_FAILURE);
            }

            char message[4096];
            while (1)
            {
                printf("Inserire il messaggio (digita '/exit' per terminare): ");
                scanf(" %[^\n]", message);
        
                if (strcmp(message, "/exit") == 0)
                {
                    if (write(fdut[i], message, strlen(message) + 1) == -1)
                    {
                        stampaErrore(4);
                        exit(EXIT_FAILURE);
                    }
                    break;
                }
        
                if (write(fdut[i], message, strlen(message) + 1) == -1)
                {
                    stampaErrore(4);
                    exit(EXIT_FAILURE);
                }
            }

            close(fdut[i]);
        }
    }

    return 0;
}

void stampaErrore(int tipo) {
    perror("Errore nell'esecuzione del programma.\n");
    switch (tipo) {
        case 1:
            perror("Errore: non è stata creata correttamente la named pipe.\n");
            break;

        case 2:
            perror("Errore: impossibile creare il processo figlio.\n");
            break;

        case 3:
            perror("Errore: impossibile aprire una o più named pipe.\n");
            break;

        case 4:
            perror("Errore: non è stato possibile scrivere sulla pipe.\n");

        default:
            break;
    }
    printf("Chiusura del programma...\n");
}