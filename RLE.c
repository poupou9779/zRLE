#include "RLE.h"
#include <string.h>
#include <stdlib.h>

/*
    returns RLE_OK if no problem has occured,
            RLE_FAILED_ALLOCATION if the allocation has failed,
            RLE_NULL_POINTER_ARG if any argument is a NULL pointer.
    args :
        - initial is the array of objects which is not encoded yet ;
        - size_of_elements is the size of each object of initial ;
        - number_of_elements_i is the number of objects that contain initial ;
        - encoded is a pointer on the aray that will contain the encoded objects ;
        - number_of_elements_e is a pointer on the number of elements that encoded will contain
        - compare is a pointer on a function which returns 1 if 2 elements of initial are equal, 0 if they aren't
*/
int encode_RLE(const void *initial, size_t size_of_elements, unsigned int number_of_elements_i,
        void **encoded, unsigned int *number_of_elements_e, int (*compare)(const void *, const void *))
{
    const BYTE *tmp_ptr_i = initial;
    BYTE *tmp_ptr_e;
    void *tmp_value = malloc(size_of_elements);
    unsigned int count,
                 i,
                 index = 0;
    if(initial == NULL)
        fprintf(stderr, "Error : encode_RLE : initial is a NULL pointer\n");
    else if(encoded == NULL)
        fprintf(stderr, "Error : encode_RLE : encoded is a NULL pointer\n");
    else if(number_of_elements_e == NULL)
        fprintf(stderr, "Error : encode_RLE : number_of_elements_e is a NULL pointer\n");
    else
    {
        if(tmp_value == NULL)
        {
            fprintf(stderr, "Error encode_RLE : allocation of tmp_value failed !\n");
            return RLE_FAILED_ALLOCATION;
        }
        *number_of_elements_e = 0;
        memcpy(tmp_value, initial, size_of_elements);
        for(count = 0, i = 0; i <= number_of_elements_i; ++i)
        {
            if(!(*compare)(&tmp_ptr_i[i*size_of_elements], tmp_value) || count == 255)
            {
                *number_of_elements_e += 1 + size_of_elements;
                count = 1;
                memcpy(tmp_value, &tmp_ptr_i[i*size_of_elements], size_of_elements);
            }
            else
                ++count;
        }
        tmp_ptr_e = *encoded = malloc(*number_of_elements_e * size_of_elements);

        if(*encoded == NULL)
        {
            fprintf(stderr, "Error encode_RLE : No memory has been allocated to *encoded\n");
            free(tmp_value);
            return RLE_FAILED_ALLOCATION;
        }
        memcpy(tmp_value, initial, size_of_elements);
        for(count = 0, i = 0; i <= number_of_elements_i; ++i)
        {
            if(!(*compare)(&tmp_ptr_i[i*size_of_elements], tmp_value) || count == 255)
            {
                tmp_ptr_e[index++] = (BYTE)count;
                memcpy(&tmp_ptr_e[index], tmp_value, size_of_elements);
                index += size_of_elements;
                count = 1;
                memcpy(tmp_value, &tmp_ptr_i[i*size_of_elements], size_of_elements);
            }
            else
                ++count;
        }
        free(tmp_value);
        return RLE_OK;
    }
    return RLE_NULL_POINTER_ARG;
}

/*
    returns RLE_OK if no problem has occured,
            RLE_FAILED_ALLOCATION if the allocation has failed,
            RLE_NULL_POINTER_ARG if any argument is a NULL pointer.
    args :
        - encoded is an array of encoded objects ;
        - size_of_elements is the size of an object ;
        - unsigned int numbers_of_elements_e is the number of elements that contain the array encoded ;
        - decoded is a pointer on the new array that will contain the decoded objects ;
        - number_of_elements_d is the number of elements that contains *decoded in the end.
*/
int decode_RLE(const void *encoded, size_t size_of_elements, unsigned int number_of_elements_e,
        void **decoded, unsigned int *number_of_elements_d)
{
    unsigned int i,
                 j,
                 index = 0;
    const BYTE *tmp_ptr_e = encoded;
    BYTE *tmp_ptr_d;
    if(encoded == NULL)
        fprintf(stderr, "Error decode_RLE : encoded is a NULL pointer\n");
    else if(decoded == NULL)
        fprintf(stderr, "Error decode_RLE : decoded is a NULL pointer\n");
    else if(number_of_elements_d == NULL)
        fprintf(stderr, "Error decode_RLE : number_of_elements_d is a NULL pointer");
    else
    {
        *number_of_elements_d = 0;
        for(i = 0; i < number_of_elements_e; ++i)
            *number_of_elements_d += (unsigned int)tmp_ptr_e[i*(size_of_elements+1)];

        tmp_ptr_d = *decoded = malloc(*number_of_elements_d*size_of_elements);

        if(*decoded == NULL)
        {
            fprintf(stderr, "Error decode_RLE : No memory alocated to *decoded !\n");
            return RLE_FAILED_ALLOCATION;
        }
        for(i = 0; i < number_of_elements_e; ++i)
            for(j = 0; j < (unsigned int)tmp_ptr_e[i*(size_of_elements+1)]; ++j)
            {
                memcpy(&tmp_ptr_d[size_of_elements*index], &tmp_ptr_e[i*(size_of_elements+1)+1], size_of_elements);
                ++index;
            }
        return RLE_OK;
    }
    return RLE_NULL_POINTER_ARG;
}

