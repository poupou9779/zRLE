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
    unsigned int count = 0,
                 i = 0,
                 index = 0;
    if(initial == NULL || encoded == NULL || number_of_elements_e == NULL || compare == NULL)
    {
        fprintf(stderr, "Error in encode_RLE : initial %s NULL\t\tencoded %s NULL\t\tnumber_of_elements_e %s NULL\t\tcompare %s NULL\n",
                initial == NULL ? "==" : "!=", encoded == NULL ? "==" : "!=", number_of_elements_e == NULL ? "==" : "!=", compare == NULL ? "==" : "!=");
        return RLE_NULL_POINTER_ARG;
    }
    if(tmp_value == NULL)
    {
        fprintf(stderr, "Error encode_RLE : allocation of tmp_value failed !\n");
        return RLE_FAILED_ALLOCATION;
    }
    *number_of_elements_e = 0;

    memcpy(tmp_value, initial, size_of_elements);

    for(i = 0; i <= number_of_elements_i; ++i)
    {
        if((*compare)(&tmp_ptr_i[i*size_of_elements], tmp_value) == 0 || count == 255)
        {
            *number_of_elements_e += 1;
            count = 1;
            memcpy(tmp_value, &tmp_ptr_i[i*size_of_elements], size_of_elements);
        }
        else
            ++count;
    }
    *encoded = malloc(*number_of_elements_e * size_of_elements);
    tmp_ptr_e = (BYTE *)*encoded;

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

    if(decoded == NULL || encoded == NULL || number_of_elements_d == NULL)
    {
        fprintf(stderr, "Error in decode_RLE : decoded %s NULL\t\tencoded %s NULL\t\tnumber_of_elements_d %s NULL\n",
                decoded == NULL ? "==" : "!=", encoded == NULL ? "==" : "!=", number_of_elements_d == NULL ? "==" : "!=");
        return RLE_NULL_POINTER_ARG;
    }
    *number_of_elements_d = 0;
    for(i = 0; i < number_of_elements_e; ++i)
        *number_of_elements_d += (unsigned int)tmp_ptr_e[i*(size_of_elements+1)];
    if((tmp_ptr_d = *decoded = malloc((*number_of_elements_d)*size_of_elements)) == NULL)
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

/*
    returns RLE_OK if no problem has occured,
            RLE_FAILED_ALLOCATION if any buffer allocation has failed
            RLE_NULL_POINTER if any argument is a NULL pointer
            the error value of encode_RLE if an error occures
    args :
        - ifile is the pre-opened binary file to encode ;
        - efile is the pre-opened binary file which will contain the encoded ifile file ;
        - size_of_elements is the size of any element in the ifile file ;
        compare is the callback funtion that will ba used by encode_RLE.
*/
int encode_file_RLE(FILE *ifile, FILE *efile, size_t size_of_elements, int (*compare)(const void *, const void *))
{
    BYTE *buffer = malloc(LENGTH*size_of_elements),
          *encoded;
    size_t ret,
           tmp_fwrite;
    unsigned int number_of_encoded_elements;
    int tmp;
    if(buffer == NULL)
    {
        fprintf(stderr, "Error encode_file_RLE : buffer non allocated\n");
        return RLE_FAILED_ALLOCATION;
    }
    else if(ifile == NULL)
        fprintf(stderr, "Error encode_file_RLE : ifile is a NULL pointer\n");
    else if(efile == NULL)
        fprintf(stderr, "Error encode_file_RLE : efile is a NULL pointer\n");
    if(compare == NULL)
        fprintf(stderr, "Error encode_file_RLE : compare is a NULL pointer\n");
    else
    {
        while((ret = fread(buffer, size_of_elements, LENGTH, ifile)) > 0)
        {
            if((tmp = encode_RLE(buffer, size_of_elements, ret, (void **)&encoded, &number_of_encoded_elements, compare)) == RLE_OK)
            {
                if((tmp_fwrite = fwrite(encoded, size_of_elements, number_of_encoded_elements, efile)) != number_of_encoded_elements)
                {
                    fprintf(stderr, "Error encode_file_RLE : writting error : too %s elements written !\n",
                            number_of_encoded_elements - tmp_fwrite > 0 ? "few" : "many");
                    return RLE_WRITING_ERROR;
                }
                else
                    free(encoded);
            }
            else
            {
                fprintf(stderr, "Error encode_file_RLE : Unable to encode %s\n", buffer);
                return tmp;
            }
        }
        return RLE_OK;
    }
    return RLE_NULL_POINTER_ARG;
}

/*
    returns RLE_OK if no problem has occured,
            RLE_FAILED_ALLOCATION if any buffer allocation has failed
            RLE_NULL_POINTER if any argument is a NULL pointer
            the error value of decode_RLE if an error occures
    args :
        - efile is the pre-opened binary file to decode ;
        - dfile is the pre-opened binary file which will contain the decoded efile file ;
        - size_of_elements is the size of any element in the efile file.
*/
int decode_file_RLE(FILE *efile, FILE *dfile, size_t size_of_elements)
{
    BYTE *buffer = malloc(LENGTH*(size_of_elements+1)),
          *tmp_decoded;
    size_t ret,
           tmp_fwrite;
    int tmp;
    unsigned int number_of_decoded_elements;
    if(buffer == NULL)
    {
        fprintf(stderr, "Error decode_file_RLE : buffer non allocated\n");
        return RLE_FAILED_ALLOCATION;
    }
    else if(efile == NULL)
        fprintf(stderr, "Error decode_file_RLE : efile is a NULL pointer\n");
    else if(dfile == NULL)
        fprintf(stderr, "Error decode_file_RLE : dfile is a NULL pointer\n");
    else
    {
        while((ret = fread(buffer, size_of_elements+1, LENGTH, efile)) > 0)
        {
            if((tmp = decode_RLE(buffer, size_of_elements, ret, (void **)&tmp_decoded, &number_of_decoded_elements)) == RLE_OK)
            {
                if((tmp_fwrite = fwrite(tmp_decoded, size_of_elements, number_of_decoded_elements, dfile)) != number_of_decoded_elements)
                {
                    fprintf(stderr, "Error decode_file_RLE : writing error : too %s elements written !\n",
                            number_of_decoded_elements - tmp_fwrite > 0 ? "few" : "many");
                    return RLE_WRITING_ERROR;
                }
                else
                    free(tmp_decoded);
            }
            else
            {
                fprintf(stderr, "Error decode_file_RLE : Unable to decode %s\n", buffer);
                return tmp;
            }
        }
        return RLE_OK;
    }
    return RLE_NULL_POINTER_ARG;
}

