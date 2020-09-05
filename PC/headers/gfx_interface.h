#ifndef GFX_INTERFACE_H
#define GFX_INTERFACE_H

#include <raylib.h>
#include <stdint.h>

typedef struct
{
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
} Rotation;

// Higher precision is needed for calculating rotation per frame
typedef struct
{
    float roll;
    float pitch;
    float yaw;
} Rotation_f;
/*
 *  @brief Draws model loaded earlier, another func to keep everythin clin xD
 */
void gyDrawModel(Vector3 rotation, Camera3D* camera, Model* model);

/*
 *  @brief Simple initialization to save space in main.c file
 */
void gyInitViewPort(int screenWidth, int screenHeight, int cameraZoom, Camera3D* cam);

/*
 *  @brief Converts array of data from server to roatation struct
 *
 *  @param [in] array - pointer to buffer written by server
 *
 *  @param [in] size - size of array (in bytes)
 *
 *  @param [out] out_rot_buff - user allocated struct to write result
 */
void gyArray2Rotation(char* array, unsigned size, Rotation* out_rot_buff);

#endif /* GFX_INTERFACE_H */