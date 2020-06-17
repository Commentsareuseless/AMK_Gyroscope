#ifndef GFX_INTERFACE_H
#define GFX_INTERFACE_H

#include <raylib.h>

/*
 *  @brief Draws model loaded earlier, another func to keep everythin clin xD
 */
void gyDrawModel(Vector3 rotation, Camera3D* camera, Model* model);

/*
 *  @brief Simple initialization to save space in main.c file
 */
void gyInitViewPort(int screenWidth, int screenHeight, int cameraZoom, Camera3D* cam);

#endif /* GFX_INTERFACE_H */