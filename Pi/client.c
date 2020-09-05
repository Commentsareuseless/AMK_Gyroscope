#include "client.h"
#include <sys/time.h>
#include <errno.h>

static struct sockaddr_in SrvAddress, CliAddress;
static sockHandle_t connectFD;
static struct timeval timeout;

int gyInitComunication()
{
    int errCheck;
    socklen_t cli_addr_len = 0;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    if((connectFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nCannot aquire socket :(\n");
        return -1;
    }

    // Set timeout on socket for retransmission
    if((setsockopt(connectFD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) < 0)
    {
        perror("\nFailed to set timeout on socket :(\n");
    }

    memset(&SrvAddress, 0, sizeof(SrvAddress));
    SrvAddress.sin_family = AF_INET;
    SrvAddress.sin_port = htons(PORT);

    if(inet_pton(AF_INET, SRV_ADDR, &SrvAddress.sin_addr) < 1)
    {
        printf("\nError converting addresses\n");
        close(connectFD);
        return -1;
    }

    if(bind(connectFD, (struct sockaddr*)&SrvAddress, sizeof(SrvAddress)) < 0)
    {
        perror("Binding failed: ");
        close(connectFD);
        return -ENOTCONN;
    }


    printf("\nSuccesful initialization!\n");
    printf("Connected to: %s on port: %d\n", inet_ntoa(SrvAddress.sin_addr), SrvAddress.sin_port);
    return CLIENT_OK;
}

int gyWaitForDataReq()
{
    const char probeRequest[] = "REQ";
    const char buff[BUFF_SIZE];
    socklen_t addr_struct_len = sizeof(CliAddress);
    ssize_t bytes_recvd = 0;
    int max_attempts = 0;

    while (strcmp(buff, probeRequest) != 0)
    {
        // Repeat requests untill we get answer
        bytes_recvd = recvfrom(connectFD,
                                (void*)buff,
                                sizeof(buff),
                                MSG_WAITALL,
                                (struct sockaddr*)&CliAddress,
                                &addr_struct_len);
        if (bytes_recvd > 0)
            printf("Received: %d %s\n", bytes_recvd, buff);

        ++max_attempts;
        if (max_attempts >= MAX_RETRYS)
            return -ENOTCONN;
    }
    return CLIENT_OK;
}

int gySendData(const char* buff, unsigned size)
{
    int numB = 0;
    numB = sendto(connectFD,
                    (void*)buff,
                    size,
                    0,
                    (struct sockaddr*)&CliAddress,
                    sizeof(CliAddress));
    printf("Data sent %s , %d\n", buff, numB);
    return CLIENT_OK;
}

void gyCleanup()
{
    close(connectFD);
}