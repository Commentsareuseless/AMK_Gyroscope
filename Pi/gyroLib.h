#ifndef GYRO_LIB_H
#define GYRO_LIB_H

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdint.h>

///////////////////////////////////////////////////////
// Config options
///////////////////////////////////////////////////////

#define GYRO_ADDR 0x69    //Check on i2cdetect -y 1

#define STATUS_REG_DATA_RDY (1 << 3)  

// Zero level voltages offset
#define ZL_off_250dps   10
#define ZL_off_500dps   15
#define ZL_off_2000dps  75

// Sensitivity  dps/LSB 
#define SENS_250dps     0.00875f
#define SENS_500dps     0.0175f
#define SENS_2000dps    0.07f

// Handle for I2C interface
typedef int I2C_fd_t;

typedef enum
{
    SCALE_2000DPS = 0x2,
    SCALE_500D_DPS = 0x1,
    SCALE_250DPS = 0x0
} DPS_Scale_t;

typedef enum
{
    DATARATE_100Hz_12_5     =0x0,
    DATARATE_100Hz_25       =0x1,
    DATARATE_200Hz_12_5     =0x4,
    DATARATE_200Hz_25       =0x5,
    DATARATE_200Hz_50       =0x6,
    DATARATE_200Hz_70       =0x7,
    DATARATE_400Hz_20       =0x8,
    DATARATE_400Hz_25       =0x9,
    DATARATE_400Hz_50       =0xa,
    DATARATE_400Hz_110      =0xb,
    DATARATE_800Hz_30       =0xc,
    DATARATE_800Hz_35       =0xd,
    DATARATE_800Hz_50       =0xe,
    DATARATE_800Hz_110      =0xf
} Datarate_t;

typedef enum
{
    WHO_AM_I = 0xf,

    CTRL_REG1 = 0x20,
    CTRL_REG2,
    CTRL_REG3,
    CTRL_REG4,
    CTRL_REG5,
    REFERENCE,
    OUT_TEMP,   // 8 bit temperature reading
    STATUS_REG,
    OUT_X_L,    //
    OUT_X_H,    //  Gyroscope 16bit data 
    OUT_Y_L,    //  
    OUT_Y_H,    //
    OUT_Z_L,    //
    OUT_Z_H,    //
    FIFO_CTRL_REG,
    FIFO_SRC_REG,
    INT1_CFG,
    INT1_SRC,
    INT1_TSH_XH,
    INT1_TSH_XL,
    INT1_TSH_YH,
    INT1_TSH_YL,
    INT1_TSH_ZL,
    INT1_TSH_ZH,
    INT1_DURATION
} Registers_t;
/////////////////////////////////////////////////////////
// Utility
/////////////////////////////////////////////////////////

typedef struct
{
    int16_t yaw;
    int16_t pitch;
    int16_t roll;
} Rotation;

// For debug
typedef struct 
{
    uint8_t x_H, x_L;
    uint8_t y_H, y_L;
    uint8_t z_H, z_L;    
} RawBytesRotation;


I2C_fd_t gyInitGyro(void);
float gyLerp(float min, float max, float amount);

void gyGetGyroReadings(Rotation* rot, I2C_fd_t fd);
void gyGetAverageReding(Rotation* buff, unsigned size, Rotation* result, I2C_fd_t gyroHandle);
void gyGetGyroDPS(Rotation* rot);
void gyComputeFinalRotation(Rotation* rot, Rotation* gyroRead, float deltaTime);

// For debug
void gyGetRawRegisterState(RawBytesRotation *read, I2C_fd_t gyroHandle);    

#endif // I2C_IF_H
