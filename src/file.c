/* 
 * Title: string - C: How to read an entire file into a buffer 
 * Source: https://stackoverflow.com/questions/14002954/c-how-to-read-an-entire-file-into-a-buffer/44894946#44894946
 * Author: Nominal Animal
 */
#include "graphics.h"

#include <assimp/cimport.h>
#include <stdlib.h>
#include <stdio.h>

/* Size of each input chunk to be
   read and allocate for. */
#ifndef  READALL_CHUNK
#define  READALL_CHUNK  262144
#endif

#define  READALL_OK          0  /* Success */
#define  READALL_INVALID    -1  /* Invalid parameters */
#define  READALL_ERROR      -2  /* Stream error */
#define  READALL_TOOMUCH    -3  /* Too much input */
#define  READALL_NOMEM      -4  /* Out of memory */

/* This function returns one of the READALL_ constants above.
   If the return value is zero == READALL_OK, then:
     (*dataptr) points to a dynamically allocated buffer, with
     (*sizeptr) chars read from the file.
     The buffer is allocated for one extra char, which is NUL,
     and automatically appended after the data.
   Initial values of (*dataptr) and (*sizeptr) are ignored.
*/
static int read_file(FILE *in, char **dataptr, size_t *sizeptr) {
    char  *data = NULL, *temp;
    size_t size = 0;
    size_t used = 0;
    size_t n;

    /* None of the parameters can be NULL. */
    if (in == NULL || dataptr == NULL || sizeptr == NULL) {
        return READALL_INVALID;
    }

    /* A read error already occurred? */
    if (ferror(in)) {
        return READALL_ERROR;
    }

    while (1) {
        if (used + READALL_CHUNK + 1 > size) {
            size = used + READALL_CHUNK + 1;

            /* Overflow check. Some ANSI C compilers
               may optimize this away, though. */
            if (size <= used) {
                free(data);
                return READALL_TOOMUCH;
            }

            temp = (char*)realloc(data, size);
            if (temp == NULL) {
                free(data);
                return READALL_NOMEM;
            }
            data = temp;
        }

        n = fread(data + used, 1, READALL_CHUNK, in);
        if (n == 0)
            break;

        used += n;
    }

    if (ferror(in)) {
        free(data);
        return READALL_ERROR;
    }

    temp = (char *)realloc(data, used + 1);
    if (temp == NULL) {
        free(data);
        return READALL_NOMEM;
    }
    data = temp;
    data[used] = '\0';

    *dataptr = data;
    *sizeptr = used;

    return READALL_OK;
}

size_t cg_file_read(char** src, const char* srcPath) {
    FILE* fp = NULL;
    fp = fopen(srcPath,"r");
    if (fp == NULL) {
        fprintf(stderr, "Error reading in file: %s\n", srcPath);
    }

    size_t size;
    read_file(fp, src, &size);
    fclose(fp);

    return size;
}
