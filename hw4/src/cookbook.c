#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cookbook.h"
#include "debug.h"

COOKBOOK *cb;
char *cookbook_name = "rsrc/cookbook.ckb";
char *main_rec;
RECIPE *main_recip;
int max_cooks = 1;
RECIPE_LINK *work_queue;
RECIPE_LINK *wq_pointer;

int analyze(COOKBOOK *cb);
int validate(int argc, char *argv[]);
void setup_workq(RECIPE *rec);

int cook(int argc, char *argv[]) {
    if(validate(argc, argv) == -1) {
        exit(-1);
    }
    int err = 0;

    FILE *in = fopen(cookbook_name, "r");                       //Parsing
    debug("Positive" );
    cb = parse_cookbook(in, &err);
    if(err) {
    fprintf(stderr, "Error parsing cookbook\n");
    exit(1);
    } else {
    unparse_cookbook(cb, stdout);
    }
    if(main_rec != NULL) {                                      //Finding in cookbook given recipe
        if(analyze(cb) != 0) {
            exit(-1);
        }
    }
    else {
        main_recip = (*cb).recipes;
    }

    setup_workq(main_recip);                                    //Setup Work Queue
    //debug("FOUND");
    RECIPE_LINK *rl = work_queue;
    //debug("FOUND");
    while(rl != NULL) {
        //debug("FOUND");
        debug("%s", ((RECIPE_LINK)(*rl)).name);
        rl = ((RECIPE_LINK)(*rl)).next;
    }
    // debug("%s\n", cookbook_name);
    //     debug("%d\n", max_cooks);
    //     debug("%s\n", main_rec);
    return -1;

}


int analyze(COOKBOOK *cb){
    int recipe_exists = -1;
    RECIPE *rec_check = (*cb).recipes;
    while(rec_check != NULL) {
        if(strcmp((*rec_check).name, main_rec) == 0) {
            recipe_exists = 0;
            main_recip = rec_check;
            debug("FOUND");
            break;
        }
        rec_check = (*rec_check).next;
    }
    return recipe_exists;
}

void setup_workq(RECIPE *rec) {
    //debug("%s", (*rec).name);
    RECIPE_LINK *nxt = (*rec).this_depends_on;
    if(nxt != NULL) {
        setup_workq((*nxt).recipe);
        while((*nxt).next != NULL) {
            setup_workq((*(*nxt).next).recipe);
            nxt = (*nxt).next;
        }
    }
     //debug("%s", (*rec).name);
    if(work_queue == NULL){
        //debug("%s", (*rec).name);

            // debug("%s", (*rec).name);
            RECIPE_LINK *j = malloc(sizeof(RECIPE_LINK));
            RECIPE_LINK r;
            r.name = (*rec).name;
            r.recipe = rec;
            r.next = NULL;

            int i = 1;
            //int *k = &i;
            ((*rec).state) = &i;
            debug("%d", *(int*)(*rec).state);
            *j = r;
            work_queue = j;
            wq_pointer = work_queue;
            debug("%s", (*wq_pointer).name);
    }
    else {
        debug("%s", (*rec).name);
           //  // void *p = (*rec).state;
           //  // void *lm = (void *) (*rec).state;
           //  debug("%s", (*rec).name);
           //  debug("%p", (*rec).state);
           // // int *lk = (int *) (*rec).state;
           //   int jk = *(int *)(*rec).state;
           //   debug("%p", (*rec).state);
           //   debug("%s", (*rec).name);
        if((*rec).state == NULL) {
            debug("%s", (*rec).name);
            RECIPE_LINK *j = malloc(sizeof(RECIPE_LINK));
            RECIPE_LINK d;
            d.name = (*rec).name;
            d.recipe = rec;
            d.next = NULL;
            int i = 1;
            int *k = &i;
            ((*rec).state) = (void *) k;
            //work_queue = &r;
            *j = d;
            (*wq_pointer).next = j;
            wq_pointer = j;
            debug("%s", (*wq_pointer).name);
        }
    }
}
    // if((*nxt).next != NULL) {
    //         debug("%s", (*rec).name);
    //         setup_workq((*(*nxt).next).recipe);
    //     }


int validate(int argc, char *argv[]) {
    if(argc >= 1 && argc <= 6) {
        int i = 1;
        if(strcmp(argv[0],"bin/cook") != 0) {
            return -1;
        }
        while(i < argc) {
            if(strcmp(argv[i],"-f") == 0) {
                if((i+1) < argc) {
                    cookbook_name = argv[i+1];
                    i += 2;
                }
                else{
                    return -1;
                }
            }
            else if(strcmp(argv[i],"-c") == 0) {
                if((i+1) < argc) {
                    max_cooks = atoi(argv[i+1]);
                    i += 2;
                }
                else
                    return -1;
            }
            else {
                main_rec = argv[i];
                i++;
            }
        }
        debug("%s\n", cookbook_name);
        debug("%d\n", max_cooks);
        debug("%s\n", main_rec);
        return 0;
    }
    else
        return -1;
}

