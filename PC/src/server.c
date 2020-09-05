#include "../headers/server.h"
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

static struct sockaddr_in SrvAddress;
static sockHandle_t listenFD;
static struct timeval timeout;

static char* msg_buffer;
static size_t msg_buff_len;

int gyInitComunication()
{
    // int errCheck;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    if((listenFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nCannot aquire socket :(\n");
        return -EHOSTDOWN;
    }

    // Set timeout on socket for retransmission
    if(setsockopt(listenFD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        perror("\nFailed to set timeout on socket :(\n");
    }

    memset(&SrvAddress, 0, sizeof(SrvAddress));
    SrvAddress.sin_family = AF_INET;
    SrvAddress.sin_port = htons(PORT);
    SrvAddress.sin_addr.s_addr = inet_addr(SRV_ADDR);

    if(connect(listenFD, (struct sockaddr*)&SrvAddress, sizeof(SrvAddress)) < 0)
    {
        perror("Connecting failed");
        close(listenFD);
        return -ECONNREFUSED;
    }

    printf("\nSuccesful initialization!\n");
    // printf("Connected on port: %d", SrvAddress.sin_port);
    return SRV_OK;
}

int gyGetClientData(char* buff, unsigned size)
{
    memcpy(buff, msg_buffer, size);
    return SRV_OK;
}

void* gyReceiveData(void* arg)
{
    int old, numB;
    // This thread can be cancelled any time
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    ssize_t bytes_recvd = 0;
    msg_buffer = (char*)arg;
    msg_buff_len = strlen(msg_buffer);
    socklen_t addr_len = sizeof(SrvAddress);
    const char probeRequest[] = "REQ";

    char buff[TRx_BUFF_SIZE];

    printf("Starting server thread...\n");

    // Send probe message
    numB = sendto(listenFD,
            probeRequest,
            sizeof(probeRequest),
            0,
            (struct sockaddr*)NULL,
            sizeof(SrvAddress));
    printf("Sent probe, %d\n", numB);
    while (1)
    {
        bytes_recvd =
        recvfrom(listenFD, (char*)buff, sizeof(buff), 0, (struct sockaddr*)NULL, &addr_len);


        if (bytes_recvd > 0)
        {
            printf("RecvTh: got data!\n");
            printf("%s\n", buff);
            printf("From: %s\n", inet_ntoa(SrvAddress.sin_addr));
            // Chars are 1B long so msg_buff_len is apropriate here
            memcpy(msg_buffer, buff, 32);
        }
        else if(bytes_recvd == 0)
        {
            printf("Buffor is empty\n");
            sleep(1);
        }
        else
        {
            printf("Error: %s\n", strerror(errno));
            printf("timeout: %d\n", bytes_recvd);
        }

    }
    return NULL;
}

void gyCleanup()
{
    close(listenFD);
}