#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <pthread.h>
#include "../headers/gfx_interface.h"
#include "../headers/server.h"

#define BUFF_SIZE 32

// Blends angular velocity given by gyroscope so
// model does't "jump" from one position to another
// static void _rotation_per_frame(Rotation* rot_in, Rotation_f* rot_out, float frame_time);

void _rotation_per_frame(Rotation* rot_in, Rotation_f* rot_out, float frame_time)
{
    float rot_in_roll = (float)rot_in->roll;
    float rot_in_pitch = (float)rot_in->pitch;
    float rot_in_yaw = (float)rot_in->yaw;


    if (rot_in_roll > 0 || rot_in_roll < 0)   rot_out->roll += rot_in_roll * frame_time;
    else if (rot_out->roll > 0) rot_out->roll -= 0.25f;
    else if (rot_out->roll < 0) rot_out->roll += 0.25f;

    if (rot_in_pitch > 0 || rot_in_pitch < 0)   rot_out->pitch += rot_in_pitch * frame_time;
    else if (rot_out->pitch > 0) rot_out->pitch -= 0.25f;
    else if (rot_out->pitch < 0) rot_out->pitch += 0.25f;

    if (rot_in_yaw > 0 || rot_in_yaw < 0)   rot_out->yaw += rot_in_yaw * frame_time;
    else if (rot_out->yaw > 0) rot_out->yaw -= 0.25f;
    else if (rot_out->yaw < 0) rot_out->yaw += 0.25f;
}

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
    Rotation_f actualRotation = {0};
    // gyroRotation.roll = 29.0f;

    ////////////////////////////////////////////////////////////
    // Socket communication parameters
    ////////////////////////////////////////////////////////////
    // Readonly buffer
    char Rx_buffer[BUFF_SIZE];// = {0};
    memset(Rx_buffer, 0xaa, BUFF_SIZE);
    Rx_buffer[BUFF_SIZE - 1] = '\0';
    // Server thread
    pthread_t srv_thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    ////////////////////////////////////////////////////////////
    // Initialization
    ////////////////////////////////////////////////////////////
    if (gyInitComunication() != 0)
        exit(EXIT_FAILURE);
    pthread_create(&srv_thread, &attr, gyReceiveData, (void*)Rx_buffer);
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
        gyArray2Rotation(Rx_buffer, sizeof(Rx_buffer), &gyroRotation);
        _rotation_per_frame(&gyroRotation, &actualRotation, GetFrameTime());
        //////////////////////////////////////////////////////////
        
        gyDrawModel((Vector3){actualRotation.roll, actualRotation.pitch, actualRotation.yaw}, &camera, &cubeModel);
    }

    /////////////////////////////////////////////////////////////////
    // Cleanup
    ///////////////////////////////////////////////////////////////// 
    pthread_cancel(srv_thread);
    UnloadTexture(cubeModel.materials[0].maps[MAP_DIFFUSE].texture);
    UnloadModel(cubeModel);
    CloseWindow();

    return 0;
}

