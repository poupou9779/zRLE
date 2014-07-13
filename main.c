#include <stdio.h>
#include <stdlib.h>
#include "RLE.h"

#define type int
#define compare compare_int

int compare(const void *, const void *);
int compare(const void *a, const void *b)
{
    return *((const type *)a) == *((const type *)b) ? 1 : 0;
}

int main(void)
{
    /*FILE *ifile = fopen("ifile.txt", "rb"),
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
    fclose(dfile);*/
    type tab[] = {5, 5, 5, 5, 5, 2, 2, 2, 22, 2, 45, 45, 45, 65}, /*5x5 3x2 1x22 1x2 3x45 1x65*/
        *tab_decoded = NULL;
    BYTE *tab_RLE = NULL;
    unsigned int n_elements_encoded,
                 n_elements_decoded;
    unsigned int i;

    encode_RLE(tab, sizeof(int), sizeof(tab)/sizeof(int), (void **)&tab_RLE, &n_elements_encoded, compare);
    if(decode_RLE(tab_RLE, sizeof(int), n_elements_encoded, (void **)&tab_decoded, &n_elements_decoded) != RLE_OK) {
        printf("too bad");
        return EXIT_FAILURE;
    }

    for(i = 0; i < n_elements_decoded; ++i)
        if(tab[i] != tab_decoded[i])
            printf("[i=%d] : %02d != %02d\n", i, tab[i], tab_decoded[i]);
    getchar();
    return EXIT_SUCCESS;
}

