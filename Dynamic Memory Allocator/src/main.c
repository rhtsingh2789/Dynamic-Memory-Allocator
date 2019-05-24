#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    double* ptr = sf_malloc(32);

    // void *ptr1 = sf_malloc(32);

    //   void *ptr2 = sf_malloc(32);

    //   void *ptr3 = sf_malloc(32);
    //   void *ptr4 = sf_malloc(32);
    //   void *ptr5 = sf_malloc(32);
    sf_show_heap();
    sf_realloc(ptr, 64);
    sf_show_heap();
    //  sf_free(ptr1);
    //  // sf_show_heap();
    //   sf_free(ptr2);
    //   //sf_show_heap();
    //   sf_free(ptr3);
    //   //sf_show_heap();
    //   sf_free(ptr4);
    //   sf_free(ptr5);
    // sf_show_heap();
     printf("%p\n", ptr);
    // sf_free((void *)ptr);
    //double* ptr2 = sf_malloc(5000);
    //*ptr = 320320320e-320;

    printf("%p\n", ptr);
    //printf("%p\n", ptr1);
    //printf("%p\n", ptr2);
    //sf_free(ptr);

    sf_mem_fini();

    return EXIT_SUCCESS;
}
