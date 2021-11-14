#include <stdio.h>

/*
 * Questo codice serve per mostrare come vengono utilizzati i _IO_write_ptr, _IO_write_base,
 * _IO_write_end, ... . 
 * Eseguendo il programma succede che _IO_write_base, _IO_buf_base,  _IO_read_ptr, _IO_read_base, _IO_read_end
 * puntano tutti all'elemento dopo l'ultimo letto, quindi il primo inserito ('S'). _IO_write_ptr
 * invece punta all'elemento dopo l'ultimo inserito.
 * 
 * Ciò avviene più o meno anche in tutti gli altri casi, infatti:
 *      _IO_write_ptr - _IO_read_ptr == 41 
 * La distanza tra i 2 puntatori (a dire il vero tra _IO_write_ptr e quelli elencati sopra) 
 * rimane sempre il numero di caratteri inseriti. (Non sempre 41 dato che poi l'output aumenta
 * di 1 cifra alla volta all'aumentare delle righe nel file)
 * 
 * L'unica cosa che mi rimane in dubbio è che se provo a stampare i caratteri DOPO il 
 * puntatore _IO_write_ptr, alla prima esecuzione del codice (file con solo le prime 2
 * righe) non c'è nulla mentre dalla volta successiva ci sono residui delle righe 
 * precedenti inserite in altre esecuzioni del programma. Non mi spiego il motivo di questo.
 */

#define MAX_WORD_SIZE 30
#define MAX_WORD_NUMBER 100
#define BUF_LIMIT (MAX_WORD_SIZE * MAX_WORD_NUMBER)

void printPtrs(FILE* file);

void printAfterWritePtr(FILE* file, int number);

void printBeforeWritePtr(FILE* file, int number);

int main()
{
    FILE* file;
    char input;
    char outputBuffer[BUF_LIMIT];
    int bufCounter;

    file = fopen("test.txt", "a+");
    if ( file == NULL )
    {
        printf("null file pointer!\n");
        return 0;
    }

    bufCounter = 0;
    while (!feof(file) 
            && ( input = getc(file) ) != EOF
            && bufCounter < BUF_LIMIT - 1)
        outputBuffer[bufCounter++] = input;

    outputBuffer[bufCounter] = '\0';
    printf("================================== FILE CONTENT ==================================\n");
    printf("%s\n\n", outputBuffer);

    printf("\n============================= BEFORE PRINTING ON FILE =============================\n");
    printPtrs(file);

    printf("\n\n\n-------------------------------- LOOKING AT WRITE PTR --------------------------------\n");
    printBeforeWritePtr(file, 10);
    printf("File write ptr (_IO_write_ptr)   %p points to %c", file -> _IO_write_ptr, *(file -> _IO_write_ptr));
    printAfterWritePtr(file, 10);


    printf("----------------------------------- APPENDING -----------------------------------\n");
    printf("appending to file: \"STOP\\n {number} characters have been typed so far\"\n\n");
    fprintf(file, "STOP\n%d characters have been typed so far", bufCounter);

    printf("\n\n============================= AFTER PRINTING ON FILE =============================\n");
    printPtrs(file);

    printf("\nN.B: _IO_write_ptr - _IO_read_ptr == %ld\n", (file -> _IO_write_ptr) -  (file -> _IO_read_ptr));

    printBeforeWritePtr(file, 41);
    printf("File write ptr (_IO_write_ptr)   %p points to %c\n", file -> _IO_write_ptr, *(file -> _IO_write_ptr));
    printAfterWritePtr(file, 10);


}

void printPtrs(FILE* file)
{

    printf("WRITE POINTERS\n");
    printf("File write ptr (_IO_write_ptr)   %p points to %c\n", file -> _IO_write_ptr, *(file -> _IO_write_ptr));
    printf("\t\t In the prev position there is '%c' and in the follw '%c'\n", *(file -> _IO_write_ptr - 1), *(file -> _IO_write_ptr + 1));
    printf("File write base (_IO_write_base) %p points to %c (whose int is: %d)\n", file -> _IO_write_base, *(file -> _IO_write_base), *(file -> _IO_write_base));
    printf("File write end (_IO_write_end)   %p points to %c (whose int is: %d)\n", file -> _IO_write_end, *(file -> _IO_write_end), *(file -> _IO_write_end));
    printf("File buf base (_IO_buf_base)     %p points to %c (whose int is: %d)\n", file -> _IO_buf_base, *(file -> _IO_buf_base), *(file -> _IO_buf_base));
    printf("File buf end (_IO_buf_end)       %p points to %c (whose int is: %d)\n", file -> _IO_buf_end, *(file -> _IO_buf_end), *(file -> _IO_buf_end));

    printf("\nNOTE: _IO_buf_base - _IO_write_base = %ld\n", file -> _IO_buf_base - file -> _IO_write_base);
    printf("NOTE: _IO_buf_end - _IO_write_end = %ld\n\n", file -> _IO_buf_end - file -> _IO_write_end);

    printf("READ POINTERS\n");
    printf("File read ptr (_IO_read_ptr)    %p points to %c\n", file -> _IO_read_ptr, *(file -> _IO_read_ptr));
    printf("File read base (_IO_read_base) %p points to %c (whose int is: %d)\n", file -> _IO_read_base, *(file -> _IO_read_base), *(file -> _IO_read_base));
    printf("File read end (_IO_read_end)   %p points to %c (whose int is: %d)\n", file -> _IO_read_end, *(file -> _IO_read_end), *(file -> _IO_read_end));

}

void printAfterWritePtr(FILE* file, int number)
{
    int counter;
    char currentChar;
    char buffer[number + 1];

    printf("\n");
    printf("%dst characters after the write pointer are: ", number);
    for (counter = 1; counter <= number; counter++)
    {
        currentChar = *(file -> _IO_write_ptr + counter);
        if (currentChar == '\n') buffer[counter - 1] = '\n';
        else buffer[counter - 1] = currentChar;
    }

    buffer[number] = '\0';
    printf("%s", buffer);
    printf("\n");
}

void printBeforeWritePtr(FILE* file, int number)
{
    int counter;
    char currentChar;
    char buffer[number + 1];

    printf("\n");
    printf("%dst characters before the write pointer are: ", number);
    for (counter = number; counter >= 1; counter--)
    {
        currentChar = *(file -> _IO_write_ptr - counter);
        if (currentChar == '\n') buffer[number - counter] = '_';
        else buffer[number - counter] = currentChar;
    }

    buffer[number] = '\0';
    printf(" %s", buffer);

    printf("\n");
}