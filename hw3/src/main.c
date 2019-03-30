#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    double* ptr = sf_malloc(32);
    double* ptr1 = sf_malloc(4000);
    printf("%p\n", ptr);
    sf_free((void *)ptr);
    //double* ptr2 = sf_malloc(5000);
    //*ptr = 320320320e-320;

    printf("%p\n", ptr);
    printf("%p\n", ptr1);
    //printf("%p\n", ptr2);
    //sf_free(ptr);

    sf_mem_fini();

    return EXIT_SUCCESS;
}
