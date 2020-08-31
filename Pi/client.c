#include "client.h"
#include <sys/time.h>
#include <errno.h>

static struct sockaddr_in PCaddress;
static sockHandle_t sockFD;
static struct timeval timeout;

int gyInitComunication()
{
    int errCheck;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if((sockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nCannot aquire socket :(\n");
        return -1;
    }

    // Set timeout on socket for retransmission
    if((setsockopt(sockFD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) < 0)
    {
        perror("\nFailed to set timeout on socket :(\n");
    }

    memset(&PCaddress, 0, sizeof(PCaddress));
    PCaddress.sin_family = AF_INET;
    PCaddress.sin_port = htons(PORT);
    
    if((errCheck = inet_pton(AF_INET, PC_ADDR, &PCaddress.sin_addr.s_addr)) < 1)
    {
        printf("\nError converting addresses\n");
        close(sockFD);
        return -1;
    }

    printf("\nSuccesful initialization!\n");
    return CLIENT_OK;
}

int gyWaitForDataReq()
{
    const char probeMessage[] = "RDY";
    const char probeRequest[] = "REQ";
    const char buff[BUFF_SIZE];
    socklen_t addr_struct_len = sizeof(PCaddress);
    ssize_t bytes_recvd = 0;
    int max_attempts = 0;

    while (strcmp(buff, probeRequest) != 0)
    {        
        // Repeat requests untill we get answer
        sendto(sockFD,
                (void*)probeMessage,
                sizeof(probeMessage),
                MSG_CONFIRM,
                (struct sockaddr*)&PCaddress,
                addr_struct_len);
        bytes_recvd = recvfrom(sockFD,
                                (void*)buff,
                                sizeof(buff),
                                MSG_WAITALL,
                                (struct sockaddr*)&PCaddress,
                                &addr_struct_len);
        if (bytes_recvd > 0)
            printf("Received: %d %s\n", bytes_recvd, buff);

        ++max_attempts;
        if (max_attempts => MAX_RETRYS)
            return -ENOTCONN;
    }
    return CLIENT_OK;
}

int gySendData(const char* buff, unsigned size)
{
    socklen_t addr_struct_len = sizeof(PCaddress);
    sendto(sockFD,
            (void*)buff,
            size,
            MSG_CONFIRM,
            (struct sockaddr*)&PCaddress,
            addr_struct_len);
}