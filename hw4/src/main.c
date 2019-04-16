#include <stdlib.h>
#include <stdio.h>

#include "cookbook.h"
#include "debug.h"

int cook(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    COOKBOOK *cbp;
    int err = 0;
    FILE *in = fopen("rsrc/hello_world.ckb", "r");
    cbp = parse_cookbook(in, &err);
    if(err) {
	fprintf(stderr, "Error parsing cookbook\n");
	exit(1);
    } else {
    cook(argc, argv);
	unparse_cookbook(cbp, stdout);
    }
    exit(1);
}
