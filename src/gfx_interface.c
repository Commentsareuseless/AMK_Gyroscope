#include "../headers/gfx_interface.h"
#include "raymath.h"


void gyDrawModel(Vector3 rotation, Camera3D* camera, Model* model)
{
    model->transform = MatrixRotateXYZ((Vector3){rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD});
    BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode3D(*camera);

            DrawModel(*model, (Vector3){0.0,0.0,0.0},1.0, WHITE);
            DrawGrid(25, 5);

        EndMode3D();

        DrawText("Pitch: ", 20, 20, 10, DARKGRAY);
        DrawText(FormatText("%5.1f", rotation.x), 60, 20 , 10, DARKGRAY);
        DrawText("Yaw: ", 20, 40, 10, DARKGRAY);
        DrawText(FormatText("%5.1f", rotation.y), 60, 40 , 10, DARKGRAY);

    EndDrawing();
}

void gyInitViewPort(int screenWidth, int screenHeight, int cameraZoom, Camera3D* cam)
{
    //Init camera
    cam->fovy = 45.0f;
    cam->position = (Vector3){cameraZoom, cameraZoom, cameraZoom};
    cam->target = (Vector3){0};
    cam->type = CAMERA_PERSPECTIVE;
    cam->up = (Vector3){0.0f, 1.0f, 0.0f};

    InitWindow(screenWidth, screenHeight, "Krecenie kostka");
    SetTargetFPS(60);

}
