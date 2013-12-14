#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RLE.h"

int main(void)
{
    FILE *initial_file = fopen("ifile.txt", "r"),
         *encoded_file = fopen("efile.txt", "w+"),
         *decoded_file = fopen("dfile.txt", "w");
    if(initial_file == NULL)
    {
        perror("Unable to open ifile.txt :");
        return EXIT_FAILURE;
    }
    else if(encoded_file == NULL)
    {
        perror("Unable to open efile.txt :");
        return EXIT_FAILURE;
    }
    else if(decoded_file == NULL)
    {
        perror("Unable to open dfile.txt :");
        return EXIT_FAILURE;
    }
    printf("Compressing...\n");
    encode_file_RLE(initial_file, encoded_file);
    printf("DONE !\n");

    rewind(encoded_file);

    printf("Decompressing...\n");
    decode_file_RLE(encoded_file, decoded_file);
    printf("DONE !");

    fclose(initial_file);
    fclose(encoded_file);
    fclose(decoded_file);
    return EXIT_SUCCESS;
}

