/**
 * @file main.c
 * @brief Programma per la creazione di un processo figlio e la scrittura di dati da un file in un FIFO.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BLOCK_DIM 1024 /**< Dimensione del blocco di dati da leggere/scrivere */
#define PROGRAM "writer" /**< Nome del programma da eseguire */

/**
 * @brief Funzione per la creazione di un processo figlio.
 *
 * @param program Il nome del programma da eseguire.
 * @param argument Un array di stringhe contenente gli argomenti da passare al programma.
 * @return int 0 in caso di successo, -1 in caso di errore.
 */
int spawn(char program[], char* argument[])
{
    int pid;

    pid = fork();
    if (pid < 0)
        return -1;
    if (pid > 0)
        return 0;
    execv(program, argument);
    abort();
}

/**
 * @brief Funzione principale del programma.
 *
 * @param argc Il numero di argomenti della riga di comando.
 * @param argv Un array di stringhe contenente gli argomenti della riga di comando.
 * @return int 0 in caso di successo, altrimenti 1.
 */
int main(int argc, char* argv[])
{
    FILE* file;
    int fifo;
    unsigned char buffer[BLOCK_DIM];
    int n;
    char* arg[3];

    if (argc != 3)
    {
        printf("Uso: %s file-origine file-destinazione\r\n", argv[0]);
        return 0;
    }

    // Allocazione e copia dei parametri per il processo figlio
    arg[0] = (char*)malloc(strlen(PROGRAM) + 1);
    strcpy(arg[0], PROGRAM);
    arg[1] = (char*)malloc(strlen(argv[2]) + 1);
    strcpy(arg[1], argv[2]);
    arg[2] = NULL;

    // Creazione del processo figlio
    if (spawn(PROGRAM, arg) < 0)
    {
        printf("Errore creazione processo\r\n");
        free(arg[0]);
        free(arg[1]);
        return 0;
    }

    // Apertura del FIFO
    fifo = open("my_fifo", O_WRONLY);
    if (fifo < 0)
    {
        printf("Errore apertura FIFO\r\n");
        free(arg[0]);
        free(arg[1]);
        return 0;
    }

    // Apertura del file di origine
    file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        printf("Errore apertura file \"%s\"\r\n", argv[1]);
        close(fifo);
        free(arg[0]);
        free(arg[1]);
        return 0;
    }

    // Lettura e scrittura dei dati dal file al FIFO
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0)
        write(fifo, buffer, n);

    // Chiusura dei file e deallocazione della memoria
    fclose(file);
    close(fifo);
    free(arg[0]);
    free(arg[1]);
    return 1;
}

