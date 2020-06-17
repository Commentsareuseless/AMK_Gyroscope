#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include "../headers/gfx_interface.h"
#include "../headers/gyroLib.h"

#define BUFF_SIZE 5


int main(void)
{
    ////////////////////////////////////////////////////////////
    // Viewport parameters
    ////////////////////////////////////////////////////////////
    const int screenHeight = 420;
    const int screenWidth = 840;
    int camZoom = 10.0f;
    float deltaTime = 0.0;
    Camera3D camera = {0};
    Model cubeModel;


    ////////////////////////////////////////////////////////////
    // Gyroscope parameters
    ////////////////////////////////////////////////////////////
    Rotation buff[BUFF_SIZE] = {0};
    Rotation gyroRotation = {0};
    Rotation actualRotation = {0};
    I2C_fd_t gyroHandle;

    ////////////////////////////////////////////////////////////
    // Initialization
    ////////////////////////////////////////////////////////////
    gyroHandle = InitGyro();   
    gyInitViewPort(screenWidth, screenHeight, camZoom, &camera);

    /////////////////////////////////////////////////////////////
    // Load model to memory
    /////////////////////////////////////////////////////////////
    cubeModel = LoadModel("kloc.obj");
    cubeModel.materials[0].maps[MAP_DIFFUSE].texture = LoadTexture("lava.png");
    /////////////////////////////////////////////////////////////

    while (!WindowShouldClose())
    { 
        //////////////////////////////////////////////////////////
        // Processing gyroscope input
        //////////////////////////////////////////////////////////
        gyGetAverageReding(buff, BUFF_SIZE, &gyroRotation, gyroHandle); 
        gyGetGyroDPS(&gyroRotation);
        gyComputeFinalRotation(&actualRotation, &gyroRotation, deltaTime);
        //////////////////////////////////////////////////////////
        
        gyDrawModel((Vector3){modelRoation.roll, modelRoation.pitch, modelRoation.yaw}, &camera, &cubeModel);
    }

    /////////////////////////////////////////////////////////////////
    // Cleanup
    ///////////////////////////////////////////////////////////////// 
    UnloadTexture(cubeModel.materials[0].maps[MAP_DIFFUSE].texture);
    UnloadModel(cubeModel);
    CloseWindow();

    return 0;
}
