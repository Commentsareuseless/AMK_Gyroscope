#include "client.h"
#include "gyroLib.h"

#define GYRO_BUFF 5


int main(void)
{
    I2C_fd_t gyroHandle;
    Rotation rotBuff[GYRO_BUFF];
    Rotation gyroRotation = {0};
    Rotation finalRotation = {0};

    char sendBuff[BUFF_SIZE];
    int errorNum = 0;

    if((errorNum = gyInitComunication()) < 0)
    {
        printf("Sth went wrong :(\n Error: %s", strerror(errorNum));
        exit(EXIT_FAILURE);
    }

    if((errorNum = gyWaitForDataReq()) != 0)
    {
        printf("Waiting for PC\n Error: %s", strerror(errorNum));
        exit(EXIT_FAILURE);
    }

    gyroHandle = gyInitGyro();
    printf("######## Succesfull intialization #########\n"
           "######## Readnig data #####################\n");
    while (1)
    {
        gyGetAverageReding(rotBuff, GYRO_BUFF, &gyroRotation, gyroHandle);
        gyGetGyroDPS(&gyroRotation);
        gyComputeFinalRotation(&finalRotation, &gyroRotation, 0.375f);

        sprintf(sendBuff, "%dx%dx%d", finalRotation.roll, finalRotation.pitch, finalRotation.yaw);
        printf("\n %s \n", sendBuff);

        gySendData(sendBuff, sizeof(sendBuff));

        // Reset finalRotation so it won't be ovarriden incorrectly
        memset(&finalRotation, 0, sizeof(finalRotation));
    }

}