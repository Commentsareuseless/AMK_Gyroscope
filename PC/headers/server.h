/*
 *  Impementation of comunication interface using UDP
 */

#ifndef CLIENT_H
#define CLIENT_H

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
#define PORT 55559
// Client (PI) address
#define SRV_ADDR "192.168.1.2"

#define TRx_BUFF_SIZE 32
// Max ammount of retransmissions
#define MAX_RETRY 10
// Return no errors
#define SRV_OK 0


/*
 *  @brief Init socket and address struct
 *
 *  @retval Returns SRV_OK if succeed, standard errno if failed
 */
int gyInitComunication();

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
 *  @brief Function implementing parellel thread
 *         which waits for server data and writes it to supplied
 *         buffor
 *  @param arg - must be pointer to array of min size 32B,
 *               Use gyGetClientData() to acces data
 *
 *  @retval NULL
 *  ------------DO NOT CALL IN MAIN THREAD-----------------
 */
void* gyReceiveData(void* arg);

/*
 *  @brief Close socket after communication
 */
void gyCleanup();

#endif