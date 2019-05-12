#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "debug.h"
#include "server.h"
#include "csapp.h"
#include "server.h"


static void terminate(int status);
void sig_handler(int signal);
int initi_mazingly(FILE *file);

static char *default_maze[] = {
  "******************************",
  "***** %%%%%%%%% &&&&&&&&&&& **",
  "***** %%%%%%%%%    *   $$$$  *",
  "*           $$$$$$ $$$$$$$$$ *",
  "*##########                  *",
  "*########## @@@@@@@@@@@@@@@@@*",
  "*           @@@@@@@@@@@@@@@@@*",
  "******************************",
  NULL
};

CLIENT_REGISTRY *client_registry;

int main(int argc, char* argv[]){
      signal(SIGHUP, sig_handler);

    char **maze = NULL;
    int i = 1;
    unsigned int port;
    int fileGiven = 0;
    int pGiven = 0;
    //char **mazingly = NULL;
    int count = 0;
    debug("IDHAR");
    while(i < argc) {
      if(strcmp(argv[i], "-p") == 0) {
        port = atoi(argv[i+1]);
        pGiven = 1;
        i+=2;
      }
      else if(strcmp(argv[i], "-t") == 0) {
        FILE *file;
          file = fopen(argv[i+1], "r");
          if(file == NULL){
            exit(1);
          }
          count = 0;
          int max_size=0;
          int size=0;
          char each_line[1000];
          debug("Hello");
          while(NULL != fgets(each_line, 1000, file)) {
            size = strlen(each_line);
            if (max_size < size) {
              max_size = size;
            }
            count++;
          }
          fclose(file);
          FILE *file1 = fopen(argv[i+1], "r");
          maze = Malloc(count*sizeof(char *));
          count = 0;
          while(NULL != fgets(each_line, max_size+1, file1)){
              if('\n' == each_line[strlen(each_line)-1]){
                size = strlen(each_line)-1;
              }
              else{
                size = strlen(each_line);
              }
              debug("%d", size);
              maze[count] = Malloc(sizeof(char)*size);
              strncpy(maze[count], each_line, size);
             // debug("%s", maze[count]);
              count++;
          }

          maze[count] = Malloc(sizeof(char));
          maze[count] = '\0';


          fclose(file1);
          i+=2;
        }
        else{
          exit(1);
        }
    }
    debug("%d", count);
    if(pGiven != 1) {
      exit(0);
    }
    client_registry = creg_init();
    if(fileGiven == 1) {
      maze_init(maze);
    }
    else{
      maze_init(default_maze);
    }
    player_init();
    debug_show_maze = 1;  // Show the maze after each packet.

    // TODO: Set up the server socket and enter a loop to accept connections
    // on this socket.  For each connection, a thread should be started to
    // run function mzw_client_service().  In addition, you should install
    // a SIGHUP handler, so that receipt of SIGHUP will perform a clean
    // shutdown of the server.

    int listenfd, *connfdp;
    listenfd = Open_listenfd(port);

    while(1){
      socklen_t clientlen = sizeof(struct sockaddr_storage);
      struct sockaddr_storage clientaddr;
      connfdp = Malloc(sizeof(int));
      *connfdp = accept(listenfd, (struct sockaddr*) &clientaddr, &clientlen);
      pthread_t tid;
      int jeff = 0;
      if(jeff == 0) {
      pthread_create(&tid, NULL, mzw_client_service, connfdp);
    }
    }

    fprintf(stderr, "You have to finish implementing main() "
	    "before the MazeWar server will function.\n");

    terminate(EXIT_FAILURE);
}

/*
 * Function called to cleanly shut down the server.
 */
void terminate(int status) {
    // Shutdown all client connections.
    // This will trigger the eventual termination of service threads.
    creg_shutdown_all(client_registry);

    debug("Waiting for service threads to terminate...");
    creg_wait_for_empty(client_registry);
    debug("All service threads terminated.");

    // Finalize modules.
    creg_fini(client_registry);
    player_fini();
    maze_fini();

    debug("MazeWar server terminating");
    exit(status);
}

void sig_handler(int signal){
    if(SIGHUP == signal){
        terminate(signal);
    }
}
