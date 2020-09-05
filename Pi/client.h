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
#include <unistd.h>

typedef int sockHandle_t;

// Port number
#define PORT 55559
// Server (PC) address
#define SRV_ADDR "192.168.1.2"
// Return no errors
#define CLIENT_OK 0
// General transmission buffers size
#define BUFF_SIZE 16
// Max number of probe messages sent before returning error
#define MAX_RETRYS 10

/*
 *  @brief Init socket and address struct
 *
 *  @retval Returns SRV_OK if succeed, standard errno if failed
 */
int gyInitComunication();

/*
 *  @brief Send probe message and wait for response
 *         For cyclic "connection" testing
 *
 *  @retval Returns SRV_OK if succeed, standard errno if failed
 */
int gyWaitForDataReq();

/*
 *  @brief Sends data via earlier aquaierd socket
 *
 *  @retval Returns SRV_OK if succeed, standard errno if failed
 */
int gySendData(const char* buff, unsigned size);

/*
 *  @brief Close socket after communication
 */
void gyCleanup();

#endif