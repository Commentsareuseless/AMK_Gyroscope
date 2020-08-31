#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <pthread.h>
#include "../headers/gfx_interface.h"
#include "../headers/server.h"

#define BUFF_SIZE 16

// Blends angular velocity given by gyroscope so
// model does't "jump" from one position to another
static void _rotation_per_frame(Rotation* rot_in, Rotation* rot_out, float frame_time);

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
    Rotation gyroRotation = {0};
    Rotation actualRotation = {0};

    ////////////////////////////////////////////////////////////
    // Socket communication parameters
    ////////////////////////////////////////////////////////////
    // Readonly buffer
    char Rx_buffer[BUFF_SIZE] = {0};
    // Server thread
    pthread_t srv_thred;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    ////////////////////////////////////////////////////////////
    // Initialization
    ////////////////////////////////////////////////////////////
    gyInitViewPort(screenWidth, screenHeight, camZoom, &camera);
    if (gyInitComunication() != 0)
        exit(EXIT_FAILURE);
    pthread_create(&srv_thred, &attr, gyServerThread, (void*)Rx_buffer);

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
        gyArray2Rotation(Rx_buffer, sizeof(Rx_buffer), &gyroRotation);
        //////////////////////////////////////////////////////////
        
        gyDrawModel((Vector3){actualRotation.roll, actualRotation.pitch, actualRotation.yaw}, &camera, &cubeModel);
    }

    /////////////////////////////////////////////////////////////////
    // Cleanup
    ///////////////////////////////////////////////////////////////// 
    pthread_cancel(srv_thred);
    UnloadTexture(cubeModel.materials[0].maps[MAP_DIFFUSE].texture);
    UnloadModel(cubeModel);
    CloseWindow();

    return 0;
}
