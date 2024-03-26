/**
 * @file reader.c
 * @brief Programma per la lettura di dati da un FIFO e la scrittura su un file.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BLOCK_DIM 1024 /**< Dimensione del blocco di dati da leggere */

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
    unsigned char buffer[BLOCK_DIM];
    int n;
    int fifo;
 
    // Verifica se il numero di argomenti è corretto
    if (argc != 2)
    {
        printf("Uso: %s file\r\n", argv[0]);
        return 0;
    }

    // Apertura del FIFO in modalità di sola lettura
    fifo = open("my_fifo", O_RDONLY);
    if (fifo < 0)
    {
        printf("Errore apertura FIFO\r\n");
        return 0;
    }

    // Apertura del file di destinazione in modalità di scrittura binaria
    file = fopen(argv[1], "wb");
    if (file == NULL)
    {
        printf("Errore apertura file \"%s\"\r\n", argv[1]);
        close(fifo);
        return 0;
    }

    // Lettura dei dati dal FIFO e scrittura nel file
    while ((n = read(fifo, buffer, sizeof(buffer))) > 0)
        fwrite(buffer, 1, n, file);

    // Chiusura dei file
    close(fifo);
    fclose(file);
    return 1;
}

