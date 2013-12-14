#include <stdio.h>
#include <stdlib.h>
#include "RLE.h"

int compare_int(const void *, const void *);
void afficher_tab(int *, unsigned int);

int compare_int(const void *a, const void *b)
{
    return *((const int *)a) == *((const int *)b) ? 1 : 0;
}

void afficher_tab(int *tab, unsigned int len)
{
    printf("tableau :\n");
    unsigned int i;
    for(i = 0; i < len; ++i)
        printf("%d\n", tab[i]);
}

int main(void)
{
    int tab[10] = {2, 2, 2, 2, 8, 8, 8, 6, 6, 6},
        *tab_decoded = NULL;
    BYTE *tab_RLE_e = NULL;
    unsigned int length_RLE_e, length2;
    
    if(encode_RLE(tab, sizeof(int), 10, (void **)&tab_RLE_e, &length_RLE_e, compare_int) == RLE_OK)
        if(decode_RLE(tab_RLE_e, sizeof(int), length_RLE_e/(sizeof(int)+1), (void **)&tab_decoded, &length2) == RLE_OK)
            afficher_tab(tab_decoded, length2);
            
    free(tab_RLE_e);
    free(tab_decoded);
    return EXIT_SUCCESS;
}

