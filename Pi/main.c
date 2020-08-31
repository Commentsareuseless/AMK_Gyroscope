#include "client.h"
#include "gyroLib.h"

#define GYRO_BUFF 5
// Max packets num without keepAlive check
#define MAX_PACK_WO_KA 10

int main(void)
{
    I2C_fd_t gyroHandle;
    Rotation rotBuff[GYRO_BUFF];
    Rotation gyroRotation = {0};
    Rotation finalRotation = {0};

    char* sendBuff[BUFF_SIZE];
    int errCheck = 0;
    // Count of packets sent without keepAlive check
    unsigned packsWoKaCheck = MAX_PACK_WO_KA;

    if((errCheck = gyInitComunication()) < 0)
    {
        exit(EXIT_FAILURE);
    }

    gyroHandle = gyInitGyro();

    while (1)
    {
        if (packsWoKaCheck == MAX_PACK_WO_KA) {
            gyWaitForDataReq();
            packsWoKaCheck = 0;
        }
        ++packsWoKaCheck;

        gyGetAverageReding(rotBuff, GYRO_BUFF, gyroRotation, gyroHandle);
        gyGetGyroDPS(&gyroRotation);
        gyComputeFinalRotation(&finalRotation, &gyroRotation, 0.375f);

        sprintf(sendBuff, "%d-%d-%d", gyroRotation.roll, gyroRotation.pitch, gyroRotation.yaw);
        printf("\n %s \n", sendBuff);

        gySendData(sendBuff, sizeof(sendBuff));
    }

}