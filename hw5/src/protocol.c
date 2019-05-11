#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "debug.h"
#include "protocol.h"
#include "csapp.h"

int returnNeg= -1;
int returnZero= 0;

//sent packet to the client
int proto_send_packet(int fd, MZW_PACKET *pkt, void *data) {
    MZW_PACKET *hdr= pkt;
    void *pntr = pkt;
    if ((*(MZW_PACKET *)pntr).type == MZW_NO_PKT) {
        return returnNeg;
    }
    size_t hdr_size = sizeof(MZW_PACKET);
    uint32_t p_len = hdr->size;
    int rio_res;
    //set time to real time
    struct timespec time_stamp;
    clock_gettime(CLOCK_MONOTONIC, &time_stamp);
    (*(MZW_PACKET *)pntr).timestamp_sec = (uint32_t)time_stamp.tv_sec;
    (*(MZW_PACKET *)pntr).timestamp_nsec = (uint32_t)time_stamp.tv_nsec;
    //convert to net format
    (*(MZW_PACKET *)pntr).size = htons(hdr->size);
    (*(MZW_PACKET *)pntr).timestamp_sec = htonl((*(MZW_PACKET *)pntr).timestamp_sec);
    (*(MZW_PACKET *)pntr).timestamp_nsec = htonl((*(MZW_PACKET *)pntr).timestamp_nsec);
    if ((rio_res = write(fd, hdr, hdr_size)) <= 0) {
        if (rio_res == 0) {
            return returnNeg;
        }
        errno = EAGAIN; //set errno
        return returnNeg;
    }
    if ((data != NULL) && (p_len > 0)) {
        if ((rio_res = write(fd, data, p_len)) <= 0) {
            if (rio_res == 0) {
                return returnNeg;
            }
            errno = EAGAIN;
            return -1;
        }
    }
    return 0;
}

//receiver
int proto_recv_packet(int fd, MZW_PACKET *pkt, void **datap) {
    MZW_PACKET *hdr= pkt;
    void *pntr = pkt;
    //read in first
    size_t hdr_size = sizeof(MZW_PACKET);
    int rio_res;
    //uint32_t p_len = hdr->payload_length;
    if ((rio_res = read(fd, hdr, hdr_size)) <= 0) {
        //set errno
        if (rio_res == 0) {
            return returnNeg;
        }
        errno = EINTR;
        return returnNeg;
    }
    if ((*(MZW_PACKET *)pntr).type == MZW_NO_PKT) {
        return returnNeg;
    }
    //convert to host byte
    (*(MZW_PACKET *)pntr).size = ntohs((*(MZW_PACKET *)pntr).size);
    (*(MZW_PACKET *)pntr).timestamp_sec = ntohl((*(MZW_PACKET *)pntr).timestamp_sec);
    (*(MZW_PACKET *)pntr).timestamp_nsec = ntohl((*(MZW_PACKET *)pntr).timestamp_nsec);
    //check payload lenght to write
    uint32_t p_len = (*(MZW_PACKET *)pntr).size;
    if (p_len > 0) {
        char *data = Malloc(p_len);
        *datap = data;
        //char *data= *payload;
        if ((rio_res = read(fd, data, p_len)) <= 0) {
            if (rio_res == 0) {
                Free(data);
                return returnNeg;
            }
            errno = EINTR;
            Free(data);
            return returnNeg;
        }
       // Free(data);
    }
    return returnZero;
}