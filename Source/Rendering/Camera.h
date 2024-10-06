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
    float fov;
    float last_x;
    float last_y;
} leto_camera_t;

leto_camera_t *LetoCreateCamera(float fov);
void LetoDestroyCamera(leto_camera_t *camera);

leto_boolean_t LetoSetCameraMatrix(leto_camera_t *camera,
                                   unsigned int shader);

#endif // LETO__CAMERA_H
