#include "../headers/gyroLib.h"
#include <stdlib.h>

I2C_fd_t gyInitGyro()
{
    //Setting up gyro, L3G4200D CS pin need to be on HIGH to enable I2C
    wiringPiSetup();
    pinMode(7, OUTPUT);
    digitalWrite(7, 1);

    I2C_fd_t gyroHandle;
    gyroHandle = wiringPiI2CSetup(GYRO_ADDR);
    int dpsScale = 0x1; // Set sensitivity

    wiringPiI2CWriteReg8(gyroHandle, CTRL_REG1, 0xf);   //Set normal mode
    wiringPiI2CWriteReg8(gyroHandle, CTRL_REG4, 0x8);   //Don't override values untill LSB and MSB are read

    return gyroHandle;
}

void gyGetRawRegisterState(RawBytesRotation *read, I2C_fd_t fd)
{
    int statRegVal = 0;
    while(!(STATUS_REG_DATA_RDY & statRegVal))
    {
        statRegVal = wiringPiI2CReadReg8(fd, STATUS_REG);
    }

    read->x_L = wiringPiI2CReadReg8(fd, OUT_X_L);
    read->x_H = wiringPiI2CReadReg8(fd, OUT_X_H);   
    
    read->y_L = wiringPiI2CReadReg8(fd, OUT_Y_L);
    read->y_H = wiringPiI2CReadReg8(fd, OUT_Y_H);   
    
    read->z_L = wiringPiI2CReadReg8(fd, OUT_Z_L);
    read->z_H = wiringPiI2CReadReg8(fd, OUT_Z_H);   
}


void gyGetGyroReadings(Rotation* rot, I2C_fd_t fd)
{
    int statRegVal = 0;
    while(!(STATUS_REG_DATA_RDY & statRegVal))
    {
        statRegVal = wiringPiI2CReadReg8(fd, STATUS_REG);
    }
    
    rot->roll = wiringPiI2CReadReg8(fd, OUT_X_L);
    rot->roll = wiringPiI2CReadReg8(fd, OUT_X_H) << 8;   
    
    rot->pitch = wiringPiI2CReadReg8(fd, OUT_Y_L);
    rot->pitch = wiringPiI2CReadReg8(fd, OUT_Y_H) << 8;   
    
    rot->yaw = wiringPiI2CReadReg8(fd, OUT_Z_L);
    rot->yaw = wiringPiI2CReadReg8(fd, OUT_Z_H) << 8;   

}

float gyLerp(float min, float max, float amount)
{
    return min + (max - min) * amount;
}

void gyGetAverageReding(Rotation* buff, uint8_t size, Rotation* result, I2C_fd_t gyroHandle)
{
    // *result = (Rotation){0};
    //Read a few redings to get more accurate average :)
    for(int i=0; i < size; ++i)
    {
        gyGetGyroReadings(buff + i, gyroHandle);
    } 

    int maxSampleR = 0;
    int maxSampleP = 0;
    int maxSampleY = 0;
    uint8_t samples = 1;
    for(int i=0; i < size; ++i)
    {
        // if((!buff[i].roll) || (!buff[i].pitch))
        // {
        //     continue;
        // }
        if(abs(buff[i].roll) > maxSampleR)  maxSampleR = buff[i].roll;
        if(abs(buff[i].pitch) > maxSampleP)  maxSampleP = buff[i].pitch;
        if(abs(buff[i].yaw) > maxSampleY)  maxSampleY = buff[i].yaw;

        result->roll     += buff[i].roll;
        result->pitch    += buff[i].pitch;
        result->yaw      += buff[i].yaw;
        ++samples;
    }  

    result->roll /= samples;
    result->pitch /= samples;
    result->yaw /= samples;

    result->roll += maxSampleR;
    result->pitch += maxSampleP;
    result->yaw -= maxSampleY; 
}

void gyGetGyroDPS(Rotation* rot)
{
    rot->roll = (float)rot->roll * SENS_250dps;
    if(rot->roll > 0) { rot->roll -= ZL_off_250dps;} else {rot->roll += ZL_off_250dps;}
    
    rot->pitch = (float)rot->pitch * SENS_250dps;
    if(rot->pitch > 0) { rot->pitch -= ZL_off_250dps;} else {rot->pitch += ZL_off_250dps;}
    
    rot->yaw = (float)rot->yaw * SENS_250dps;
    if(rot->yaw > 0) { rot->yaw -= ZL_off_250dps;} else {rot->yaw += ZL_off_250dps;}
}

void gyComputeFinalRotation(Rotation* rot, Rotation* gyroRead, float deltaTime)
{
    if (gyroRead->roll > 10 || gyroRead->roll < -10)
    {
        rot->roll = (int16_t)gyLerp(rot->roll, rot->roll + gyroRead->roll, deltaTime);
    }

    if (gyroRead->pitch > 10 || gyroRead->pitch < -10)
    {
         rot->pitch = (int16_t)gyLerp(rot->pitch, rot->pitch + gyroRead->pitch, deltaTime);
    }

    if (gyroRead->yaw > 10 || gyroRead->yaw < -10)
    {
        rot->yaw = (int16_t)gyLerp(rot->yaw, rot->yaw + gyroRead->yaw, deltaTime);
    }
}
