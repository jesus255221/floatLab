/*
2018.12.01 Cheng-Kung Sysprog, Chen Yi-XI
To generate different mode (e.g. normalize...) of randomized float array,
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "const.h"

#define RAND_RANGE_MAX 100000
#define RAND_RANGE_MIN 1
#define DE2NOR_RANGE_MAX 0x7FFFFF
#define DE2NOR_RANGE_MIN 0x400000

#define NORMAL_LOWER_BIT 0.01


/* generate two array, one is for float, the other is for fixed-point (implemented by unsigned int) */
int gen_rand_Array(size_t size, enum Mode mode, unsigned *float_output, unsigned *unsigned_output) {
    /* set output memory space */
    enum Error err = EXEC_SUCCESS;

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        /* get a random number to generate corresponding normalized/ denormalize float */
        unsigned int seed = RAND_RANGE_MIN + rand() % RAND_RANGE_MAX;
        unsigned de2nor_seed = DE2NOR_RANGE_MIN + rand() % DE2NOR_RANGE_MAX;
        float result;
        switch (mode) {
            case SPECIAL:
                break;
            case NORMALIZE:
                result = seed * NORMAL_LOWER_BIT;
                float_output[i] = *(unsigned *)&result;
                unsigned_output[i] = seed;
                break;
            case DENORMALIZE:
                float_output[i] = seed;
                unsigned_output[i] = seed;
                break;
            case DE2NOR:
                float_output[i] = de2nor_seed;
                unsigned_output[i] = de2nor_seed;
            default:
                break;
        }
    }
    return err;
}

int main() {
    enum Error err = EXEC_SUCCESS;
    /*
    if (argc < 2) {
        fprintf(stderr, "Failed to execute for insufficient argument.\n");
        err = INSUFFICIENT_ARGC;
        return err;
    }
    */
    
    size_t arrSize = array_size;

    /*
    if (arrSize > DENORMAL_RANGE_MAX || arrSize < DENORMAL_RANGE_MIN) {
        fprintf(stderr, "Failed to load too large array size.\n");
        err = WRONG_ARR_SIZE;
        return err;
    }
    */
    
    size_t floatMode = float_mode;

    /*
    if (floatMode < DENORMALIZE || floatMode > SPECIAL) {
        fprintf(stderr, "Failed to load wrong float mode.\n");
        err = WRONG_FLOAT_MODE;
        return err;
    }
    */

    /* although this is a float array, in denormalize float there is no way to 
       and no need to translate it into decimal form and print it all out 
       Instead, using unsigned to transfer will reduce the time to decompose. */

    unsigned *float_output;
    float_output = (unsigned *)malloc(array_size * sizeof(unsigned int));
    if (float_output == NULL) {
        err = MEM_NOT_ALLOC;
        fprintf(stderr, "Failed to malloc test case.\n");
        return err;
    }

    unsigned *unsigned_output;
    unsigned_output = (unsigned *)malloc(array_size * sizeof(unsigned int));
    err = gen_rand_Array(array_size, float_mode, float_output, unsigned_output);

    /* log out random float integer */
    FILE *fp_float_out = fopen(FLOAT_OUT_FILE_LOCAT, "w");
    if (!fp_float_out) {
        fprintf(stderr,"Failed to write float output file.\n");
        err = FILE_CANT_OPEN;
        return err;
    }

    for (int i = 0; i < arrSize; i ++){
        fprintf(fp_float_out, "%u\n", float_output[i]);
    }

    fclose(fp_float_out);


    /* log out random unsigned integer */
    FILE *fp_unsigned_out = fopen(UNSIGNED_OUT_FILE_LOCAT, "w");
    if (!fp_unsigned_out) {
        fprintf(stderr,"Failed to write unsigned output file.\n");
        err = FILE_CANT_OPEN;
        return err;
    }

    for (int i = 0; i < arrSize; i ++){
        fprintf(fp_unsigned_out, "%u\n", unsigned_output[i]);
    }

    fclose(fp_unsigned_out);

    return err;
}