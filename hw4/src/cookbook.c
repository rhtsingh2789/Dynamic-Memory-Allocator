#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cookbook.h"
#include "debug.h"

COOKBOOK *cb;
char *cookbook_name = "rsrc/cookbook.ckb";
char *main_rec;
RECIPE *main_recip;
int max_cooks = 1;
RECIPE_LINK *work_queue;
RECIPE_LINK *work_queue_new;
RECIPE_LINK *wq_pointer;
RECIPE_LINK *nwq_pointer;

int analyze(COOKBOOK *cb);
int validate(int argc, char *argv[]);
void setup_workq(RECIPE *rec);
void process(RECIPE_LINK *wq);
void cooking_here(RECIPE_LINK *wqn);

int cook(int argc, char *argv[]) {
    if(validate(argc, argv) == -1) {
        exit(-1);
    }
    int err = 0;

    FILE *in = fopen(cookbook_name, "r");                       //Parsing
    //debug("Positive" );
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

    setup_workq(main_recip);
                                   //Setup Work Queue
    RECIPE_LINK *rl = work_queue_new;
    while(rl != NULL) {
        //debug("%s", ((RECIPE_LINK)(*rl)).name);
        //debug("%d", (*(int*)(*((RECIPE_LINK)(*rl)).recipe).state));
        rl = ((RECIPE_LINK)(*rl)).next;
    }
    RECIPE_LINK *rlw = work_queue;
    while(rlw != NULL) {
        //debug("%s", ((RECIPE_LINK)(*rlw)).name);
        //debug("%d", (*(int*)(*((RECIPE_LINK)(*rlw)).recipe).state));
        rlw = ((RECIPE_LINK)(*rlw)).next;
    }

    cooking_here(work_queue_new);

    RECIPE_LINK *clean_proc = work_queue_new;
    while(clean_proc != NULL) {
        free(clean_proc);
        clean_proc = (*clean_proc).next;
    }

    clean_proc = work_queue;
    while(clean_proc != NULL) {
        free(clean_proc);
        clean_proc = (*clean_proc).next;
    }
    // cooking_here(work_queue_new);
    // rl = work_queue_new;
    // while(rl != NULL) {
    //     debug("%s", ((RECIPE_LINK)(*rl)).name);
    //     debug("%d", (*(int*)(*((RECIPE_LINK)(*rl)).recipe).state));
    //     rl = ((RECIPE_LINK)(*rl)).next;
    // }
    // rlw = work_queue;
    // while(rlw != NULL) {
    //     debug("%s", ((RECIPE_LINK)(*rlw)).name);
    //     debug("%d", (*(int*)(*((RECIPE_LINK)(*rlw)).recipe).state));
    //     rlw = ((RECIPE_LINK)(*rlw)).next;
    // }
    return -1;

}


int analyze(COOKBOOK *cb){
    int recipe_exists = -1;
    RECIPE *rec_check = (*cb).recipes;
    while(rec_check != NULL) {
        if(strcmp((*rec_check).name, main_rec) == 0) {
            recipe_exists = 0;
            main_recip = rec_check;
            //debug("FOUND");
            break;
        }
        rec_check = (*rec_check).next;
    }
    return recipe_exists;
}

void setup_workq(RECIPE *rec) {
    RECIPE_LINK *nxt = (*rec).this_depends_on;
    //int i;
    if(nxt != NULL) {
        //i = 0;
        setup_workq((*nxt).recipe);
        while((*nxt).next != NULL) {
            setup_workq((*(*nxt).next).recipe);
            nxt = (*nxt).next;
        }
    }else{
            if(work_queue_new == NULL){
                RECIPE_LINK *j = malloc(sizeof(RECIPE_LINK));
                RECIPE_LINK r;
                r.name = (*rec).name;
                r.recipe = rec;
                r.next = NULL;
                int i = 1;
                ((*rec).state) = &i;
                *j = r;
                work_queue_new = j;
                nwq_pointer = work_queue_new;
        }
        else {
            if((*rec).state == NULL) {
                RECIPE_LINK *j = malloc(sizeof(RECIPE_LINK));
                RECIPE_LINK d;
                d.name = (*rec).name;
                d.recipe = rec;
                d.next = NULL;
                int i = 1;
                ((*rec).state) = &i;
                *j = d;
                (*nwq_pointer).next = j;
                nwq_pointer = j;
            }
        }
    }
    if(work_queue == NULL && (*rec).state == NULL){
            RECIPE_LINK *j = malloc(sizeof(RECIPE_LINK));
            RECIPE_LINK r;
            r.name = (*rec).name;
            r.recipe = rec;
            r.next = NULL;
            int i = 1;
            ((*rec).state) = &i;
            *j = r;
            work_queue = j;
            wq_pointer = work_queue;
    }
    else {
        if((*rec).state == NULL) {
            RECIPE_LINK *j = malloc(sizeof(RECIPE_LINK));
            RECIPE_LINK d;
            d.name = (*rec).name;
            d.recipe = rec;
            d.next = NULL;
            int i = 1;
            ((*rec).state) = &i;
            *j = d;
            (*wq_pointer).next = j;
            wq_pointer = j;
        }
    }
}



void cooking_here(RECIPE_LINK *wqn){
    while(wqn != NULL) {
        TASK *tsk = (*(*wqn).recipe).tasks;
        while(tsk != NULL) {
            int pod = fork();
            if(pod == 0) {
                if((*(*tsk).steps).next == NULL) {
                    STEP *stp = tsk->steps;
                    //debug("Madarchdo");
                    char path[20] = "util/";
                    char pathnm[20] = "";

                    strcpy(pathnm, (*stp->words));
                    strcat(path, pathnm);
                    execvp(path, (*stp).words);
                    exit(0);
                }
                else {
                        STEP* stp = tsk->steps;
                        int N = 4;
                        int pipes[2*N];
                        int k = N - 1;
                        for(int i=0; i<k; i++) {
                            int pipo = pipe(&pipes[i*2]);
                            if(0 > pipo) {
                                exit(1);
                            }
                        }

                        int i = 0;
                        while(stp) {
                            pid_t pid = fork();
                            if(pid == 0) {
                                if(i != 0) {
                                    if(0 > dup2(pipes[(i-1)*2], STDIN_FILENO)) {
                                        exit(1);
                                    }
                                }
                                if(stp->next != NULL) {
                                    if(dup2(pipes[i*2+1], STDOUT_FILENO) < 0) {
                                        exit(1);
                                    }
                                }
                                int dub = 2*N;
                                for(int j=0; j<dub; j++) {
                                    close(pipes[j]);
                                }
                                char path[25] = "util/";;
                                int kameena = 1;
                                if(kameena == 0){
                                }
                                char strinng[25] = "";
                                strcpy(strinng, (*stp->words));
                                strcat(path, strinng);
                                int executive = execvp(path, stp->words);
                                if(executive < 0) {
                                    exit(1);
                                }
                            }
                            else if(pid < 0) {
                                exit(1);
                            }
                            stp = stp->next;
                            i++;
                        }
                        int dubly = 2*N;
                        for(int j=0; j<dubly; j++) {
                            close(pipes[j]);
                        }
                        int minus = N-1;
                        for(int j=0; j<minus; j++) {
                            wait(NULL);
                        }
                        exit(0);
                    }
            }
            else {
                wait(NULL);
                //exit(0);
            }
            tsk = (*tsk).next;
        }
        int cooked = 2;
        (*(*wqn).recipe).state = &cooked;
        process(work_queue);
        wqn = (*wqn).next;
    }
}
void process(RECIPE_LINK *wq) {
    //Cook and change the state to 2 which means it's cooked
    //Go through the work queue and find if it's sub recipes have been finished, if yes then add it to workque
    //repeat process untill all recipe is finished
    RECIPE_LINK *last = work_queue_new;

    while((*last).next != NULL) {
        last = (*last).next;
    }

    int ready = 1;
    //int cooked = 2;
    //(*(*wq).recipe).state = &cooked;
    RECIPE_LINK *pointing = wq;
    //debug("AGAIINNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
    while(pointing != NULL) {
        // debug("I'm stuck");
        ready = 1;
        RECIPE_LINK *dependecy_pointer = (*(*pointing).recipe).this_depends_on;
        if((*pointing).next != NULL){
            //debug("%s", (*(*pointing).next).name);
        }
        if((*(int*)(*(*pointing).recipe).state) != 2 &&  (*(int*)(*(*pointing).recipe).state) != 3) {
            // debug("%s", (*(*pointing).recipe).name);
        while(dependecy_pointer != NULL) {
            // debug("%s", (*(*pointing).recipe).name);
            //debug("I'm stuck");
            if((*(int*)(*(*dependecy_pointer).recipe).state) == 2) {
                //execvp();
            }
            else{
                ready = 0;
                break;
            }
            dependecy_pointer = (*dependecy_pointer).next;
        }
        if(ready == 1) {
            RECIPE_LINK *j = malloc(sizeof(RECIPE_LINK));
            RECIPE_LINK d;
            d.name = (*pointing).name;
            d.recipe = (*pointing).recipe;
            d.next = NULL;
            int i = 3;
            ((*(*pointing).recipe).state) = &i;
            *j = d;
            (*last).next = j;
            last = j;

            int processing = 3;
            (*(*pointing).recipe).state = &processing;
        }
    }
        pointing = (*pointing).next;
    }
}

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
                if(argc > (i+1)) {
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
        //debug("%s\n", cookbook_name);
        //debug("%d\n", max_cooks);
        //debug("%s\n", main_rec);
        return 0;
    }
    else
        return -1;
}

