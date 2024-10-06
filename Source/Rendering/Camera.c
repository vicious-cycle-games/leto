#include "Camera.h"
#include <CGLM/cam.h>
#include <CGLM/mat4.h>
#include <GLAD2/gl.h>
#include <Output/Errors.h>
#include <Utilities/Macros.h>

leto_camera_t *LetoCreateCamera(float fov)
{
    leto_camera_t *camera;
    LETO_ALLOC_OR_FAIL(camera, sizeof(leto_camera_t));

    camera->position[0] = 0.0f;
    camera->position[1] = 0.0f;
    camera->position[2] = 3.0f;

    camera->front[0] = 0.0f;
    camera->front[1] = 0.0f;
    camera->front[2] = -1.0f;

    camera->up[0] = 0.0f;
    camera->up[1] = 1.0f;
    camera->up[2] = 0.0f;

    camera->yaw = -90.0f;
    camera->pitch = -90.0f;
    camera->fov = fov;

    camera->last_x = 800.0f / 2.0f;
    camera->last_y = 600.0f / 2.0f;

    return camera;
}

void LetoDestroyCamera(leto_camera_t *camera)
{
    if (camera == NULL) return;
    free(camera);
}

leto_boolean_t LetoSetCameraMatrix(leto_camera_t *camera,
                                   unsigned int shader)
{
    vec3 center_vec;
    glm_vec3_add(camera->position, camera->front, center_vec);
    mat4 matrix;
    glm_lookat(camera->position, center_vec, camera->up, matrix);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "camera_view"), 1,
                       GL_FALSE, &matrix[0][0]);
    return leto_true_t; // implement checks later
}
