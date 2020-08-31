/*
 *  Impementation of comunication interface using UDP
 */

#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

typedef int sockHandle_t;

// Port number
#define PORT 6969
// Client (PI) address
#define PI_ADDR "192.168.43.40"

#define TRx_BUFF_SIZE 16
// Max ammount of retransmissions
#define MAX_RETRY 10
// Return no errors
#define SRV_OK 0
// Cannot get answer from client
#define CONN_LOST -1

/*
 *  @brief Init socket and address struct
 *
 *  @retval Returns SRV_OK if succeed, standard errno if failed
 */
int gyInitComunication();

/*
 *  @brief Listen for client's data in blocking mode 
 *         Writes internal buffor if received data, use gyGetClientData() to access it
 */
int gyListenForClient();

/*
 *  @brief TESTING
 *         Copies static buffor to given array
 *         Array must be at least 9 chars long
 *         Returns space separated array of 16b ints
 *
 *  @param buff - pointer to memory where data will be written
 *
 *  @param size - size of buff array (in Bytes)
 *
 *  @retval Returns SRV_OK if succeed
 */
int gyGetClientData(char* buff, unsigned size);

/*
 *  @brief Function implementing parellel server thread
 *         which waits for client data and writes it to supplied
 *         buffor
 *  @param arg - must be pointer to array of min size 9B,
 *               Use gyGetClientData() to acces data
 *
 *  @retval NULL
 *  ------------DO NOT CALL IN MAIN THREAD-----------------
 */
void* gyServerThread(void* arg);

#endif