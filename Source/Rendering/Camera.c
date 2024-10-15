/**
 * @file Camera.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides implementation for Leto's camera object and all of its
 * helper functions.
 * @implements Camera.h
 * @date 2024-10-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#include "Camera.h" // Public interface parent

#include <CGLM/cam.h> // GLM camera functions (lookat, etc.)
#include <GLAD2/gl.h> // OpenGL function pointers

bool LetoCreateCamera(leto_camera_t *camera, float fov, float speed,
                      float sensitivity)
{
    if (camera == NULL) return false;

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
    camera->pitch = 0.0f;
    camera->speed = speed;
    camera->sensitivity = sensitivity;
    camera->fov = fov;

    camera->last_x = 800.0f / 2.0f;
    camera->last_y = 600.0f / 2.0f;

    return camera;
}

void LetoSetCameraMatrix(leto_camera_t *camera, unsigned int shader)
{
    vec3 center_vec;
    glm_vec3_add(camera->position, camera->front, center_vec);
    mat4 matrix;
    glm_lookat(camera->position, center_vec, camera->up, matrix);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "camera_view"), 1,
                       GL_FALSE, &matrix[0][0]);
}

void LetoMoveCameraPosition(leto_camera_t *camera, float deltatime,
                            leto_movement_directions_t direction)
{
    if (camera == NULL) return;

    float speed = camera->speed * deltatime;
    switch (direction)
    {
        case forward:
            glm_vec3_muladds(camera->front, speed, camera->position);
            break;
        case backwards:
            glm_vec3_mulsubs(camera->front, speed, camera->position);
            break;
        case left:
        {
            vec3 front_up_cross;
            glm_vec3_cross(camera->front, camera->up, front_up_cross);
            glm_normalize(front_up_cross);
            glm_vec3_mulsubs(front_up_cross, speed, camera->position);
        }
        break;
        case right:
        {
            vec3 front_up_cross;
            glm_vec3_cross(camera->front, camera->up, front_up_cross);
            glm_normalize(front_up_cross);
            glm_vec3_muladds(front_up_cross, speed, camera->position);
        }
        break;
        case up:
            glm_vec3_muladds(camera->up, speed, camera->position);
            break;
        case down:
            glm_vec3_mulsubs(camera->up, speed, camera->position);
            break;
        default: break; // can't happen
    }
}

void LetoMoveCameraOrientation(leto_camera_t *camera, float x, float y)
{
    if (camera == NULL) return;

    float xoffset = x - camera->last_x;
    float yoffset = camera->last_y - y;
    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    // Constrain pitch to make sure camera doesn't flip.
    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    glm_normalize_to(
        (vec3){cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch)),
               sinf(glm_rad(camera->pitch)),
               sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch))},
        camera->front);

    camera->last_x = x;
    camera->last_y = y;
}
