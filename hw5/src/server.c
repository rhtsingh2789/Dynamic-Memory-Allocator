#include "client_registry.h"
#include "server.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "debug.h"
#include "protocol.h"
#include "csapp.h"
#include "player.h"
/*
 * Thread function for the thread that handles a particular client.
 *
 * @param  Pointer to a variable that holds the file descriptor for
 * the client connection.  This pointer must be freed once the file
 * descriptor has been retrieved.
 *
 * This function executes a "service loop" that receives packets from
 * the client and dispatches to appropriate functions to carry out
 * the client's requests.  It also maintains information about whether
 * the client has logged in or not.  Until the client has logged in,
 * only LOGIN packets will be honored.  Once a client has logged in,
 * LOGIN packets will no longer be honored, but other packets will be.
 * The service loop ends when the network connection shuts down and
 * EOF is seen.  This could occur either as a result of the client
 * explicitly closing the connection, a timeout in the network causing
 * the connection to be closed, or the main thread of the server shutting
 * down the connection as part of graceful termination.
 */
void send_response(int fd, MZW_PACKET *pkt, int type, char *payload,  int param1, int param2, int param3,  int size);
int loggedIn = 0;
CLIENT_REGISTRY *client_registry;
int debug_show_maze;
void *mzw_client_service(void *arg){
    int fd = *(int *)arg;
    Free(arg);
    Pthread_detach(pthread_self());
    creg_register(client_registry, fd);
    PLAYER *player ;
    while(1){
        MZW_PACKET *pkt = Malloc(sizeof(MZW_PACKET));
        void *pntr = pkt;
        void** pl = Malloc(sizeof(char*));
        int chk;
        if((chk = proto_recv_packet(fd, pkt, pl)) == -1){
            player_check_for_laser_hit(player);
        }
        if((*(MZW_PACKET *)pntr).type == MZW_LOGIN_PKT){
            loggedIn = 1;
            player = player_login(fd, (*(MZW_PACKET *)pntr).param1, *pl);
            if(player!=NULL){  //logged in and ready
                int z = 0;
                send_response( fd,  pkt, MZW_READY_PKT, *pl, (*(MZW_PACKET *)pntr).param1, z, z, (*(MZW_PACKET *)pntr).size);
                player_reset(player); //successful login
            }
            else{
                int z = 0;
                send_response(fd, pkt, MZW_INUSE_PKT, NULL, z, z, z, z);
                //exit(1);
            }
        }
        else if(loggedIn == 1 && (*(MZW_PACKET *)pntr).type == MZW_MOVE_PKT){
            player_move(player, (*(MZW_PACKET *)pntr).param1);
            send_response(fd, pkt, MZW_MOVE_PKT, *pl, (*(MZW_PACKET *)pntr).param1, (*(MZW_PACKET *)pntr).param2, (*(MZW_PACKET *)pntr).param3, (*(MZW_PACKET *)pntr).size);
        }
        else if(loggedIn == 1 && (*(MZW_PACKET *)pntr).type == MZW_TURN_PKT){
            player_rotate(player, (*(MZW_PACKET *)pntr).param1);
            send_response(fd, pkt, MZW_TURN_PKT, *pl, (*(MZW_PACKET *)pntr).param1, (*(MZW_PACKET *)pntr).param2, (*(MZW_PACKET *)pntr).param3, (*(MZW_PACKET *)pntr).size);

        }
        else if(loggedIn == 1 && pkt->type == MZW_FIRE_PKT){
            player_fire_laser(player);
            int z = 0;
            send_response(fd, pkt, MZW_FIRE_PKT, *pl, z, z, z, (*(MZW_PACKET *)pntr).size);
        }
        else if(loggedIn == 1 && (*(MZW_PACKET *)pntr).type == MZW_REFRESH_PKT){
            player_invalidate_view(player);
            player_update_view(player);
            send_response(fd, pkt, MZW_REFRESH_PKT, *pl, (*(MZW_PACKET *)pntr).param1, (*(MZW_PACKET *)pntr).param2, (*(MZW_PACKET *)pntr).param3, (*(MZW_PACKET *)pntr).size);
        }
        else if(loggedIn == 1 && pkt->type == MZW_CHAT_PKT){
            player_send_chat(player, *pl, pkt->size);
            send_response(fd, pkt, MZW_CHAT_PKT, *pl, (*(MZW_PACKET *)pntr).param1, (*(MZW_PACKET *)pntr).param2, (*(MZW_PACKET *)pntr).param3, (*(MZW_PACKET *)pntr).size);
        }
        else{
            continue;
        }
        show_maze();
        Free(pl);
        Free(pkt);
    }
    creg_shutdown_all(client_registry);
    return NULL;
}

void send_response(int fd,MZW_PACKET *pkt, int type, char *payload,  int param1, int param2, int param3,  int size){
    MZW_PACKET *response = Malloc(sizeof(MZW_PACKET));
    void *pntr = response;
    memset(response, 0, sizeof(MZW_PACKET));
    (*(MZW_PACKET *)pntr).type = type;
    (*(MZW_PACKET *)pntr).size = size;
    (*(MZW_PACKET *)pntr).param1 = param1;
    (*(MZW_PACKET *)pntr).param2 = param2;
    (*(MZW_PACKET *)pntr).param3 = param3;
    (*(MZW_PACKET *)pntr).timestamp_sec = (*(MZW_PACKET *)pntr).timestamp_sec;
    (*(MZW_PACKET *)pntr).timestamp_nsec = (*(MZW_PACKET *)pntr).timestamp_nsec;

    proto_send_packet(fd, response, payload);

    Free(response);
}