#ifndef LETO__CAMERA_H
#define LETO__CAMERA_H

#include <CGLM/vec3.h>
#include <Utilities/Types.h>

typedef struct leto_camera
{
    vec3 position;
    vec3 front;
    vec3 up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float fov;
    float last_x;
    float last_y;
} leto_camera_t;

typedef enum leto_movement_directions
{
    forward,
    backwards,
    left,
    right,
    up,
    down
} leto_movement_directions_t;

leto_camera_t *LetoCreateCamera(float fov, float speed, float sensitivity);
void LetoDestroyCamera(leto_camera_t *camera);

leto_boolean_t LetoSetCameraMatrix(leto_camera_t *camera,
                                   unsigned int shader);

void LetoMoveCameraPosition(leto_camera_t *camera, float deltatime,
                            leto_movement_directions_t direction);
void LetoMoveCameraOrientation(leto_camera_t *camera, float x, float y);

#endif // LETO__CAMERA_H
