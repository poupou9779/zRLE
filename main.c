#include <stdio.h>
#include <stdlib.h>
#include "RLE.h"

int compare_char(const void *, const void *);
int compare_char(const void *a, const void *b)
{
    return *((const char *)a) == *((const char *)b) ? 1 : 0;
}

int main(void)
{
    FILE *ifile = fopen("ifile.txt", "rb"),
         *cfile = fopen("efile.txt", "wb+"),
         *dfile = fopen("dfile.txt", "wb");

    printf("Encoding...\n");
    encode_file_RLE(ifile, cfile, sizeof(char), compare_char);
    rewind(cfile);
    printf("Done !\nDecoding...\n");
    decode_file_RLE(cfile, dfile, sizeof(char));
    printf("Done !\n");

    fclose(ifile);
    fclose(cfile);
    fclose(dfile);
    return EXIT_SUCCESS;
}
