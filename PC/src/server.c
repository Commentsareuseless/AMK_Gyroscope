#include "server.h"
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

static struct sockaddr_in PIaddress;
static struct sockaddr_in SrvAddress;
static sockHandle_t sockFD;
static struct timeval timeout;

static char* msg_buffer;
static size_t msg_buff_len;

int gyInitComunication()
{
    int errCheck;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    if((sockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nCannot aquire socket :(\n");
        return -EHOSTDOWN;
    }

    // Set timeout on socket for retransmission
    if(setsockopt(sockFD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        perror("\nFailed to set timeout on socket :(\n");
    }

    memset(&PIaddress, 0, sizeof(PIaddress));
    memset(&SrvAddress, 0, sizeof(SrvAddress));
    SrvAddress.sin_family = AF_INET;
    SrvAddress.sin_port = htons(PORT);
    
    if((errCheck = inet_pton(AF_INET, PI_ADDR, &SrvAddress.sin_addr.s_addr)) < 1)
    {
        printf("\nError converting addresses\n");
        close(sockFD);
        return -EHOSTUNREACH;
    }

    if((errCheck = bind(sockFD, (const struct sockaddr*)&SrvAddress, sizeof(SrvAddress))) < 0)
    {
        perror("Binding failed");
        close(sockFD);
        return -ECONNREFUSED;
    }

    printf("\nSuccesful initialization!\n");
    return SRV_OK;
}

int gyGetClientData(char* buff, unsigned size)
{
    memcpy(buff, msg_buffer, size);
    return SRV_OK;
}

void* gyServerThread(void* arg)
{
    int old;
    // This thread can be cancelled any time
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    ssize_t bytes_recvd = 0;
    msg_buffer = (char*)arg;
    msg_buff_len = strlen(msg_buffer);

    const char probeRequest[] = "REQ";
    const char probeResponse[] = "RDY";
    char buff[TRx_BUFF_SIZE];
    socklen_t addr_struct_len = sizeof(PIaddress);

    printf("Starting server thread...\n");

    while (1)
    {
        bytes_recvd = recvfrom(sockFD,
                                (void*)buff,
                                sizeof(buff),
                                MSG_WAITALL,
                                (struct sockaddr*)&PIaddress,
                                &addr_struct_len);
        // Send keep alive message on request
        if (strcmp(buff, probeResponse) == 0)
        {
            sendto(sockFD,
                   (void*)probeRequest,
                   sizeof(probeRequest),
                   MSG_CONFIRM,
                   (struct sockaddr*)&PIaddress,
                   sizeof(PIaddress));
            bytes_recvd = 0;
            continue;
        }

        if (bytes_recvd > 0)
        {
            printf("SrvTh: got data!\n");
            printf("%s\n", buff);
            // Chars are 1B long so msg_buff_len is apropriate here
            memcpy(msg_buffer, buff, msg_buff_len);
        }
    }
    return NULL;
}