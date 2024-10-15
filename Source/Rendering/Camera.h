/**
 * @file Camera.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides an interface for controlling and setting values having
 * to do with the camera.
 * @date 2024-10-14
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__CAMERA_H
#define LETO__CAMERA_H

// GLM 3D vectors.
#include <CGLM/vec3.h>

/**
 * @brief A container around the various positions and scalars that make up
 * the player's current position.
 */
typedef struct leto_camera
{
    /**
     * @brief A 3 dimensional vector describing where in worldspace the
     * player currently is.
     */
    vec3 position;
    /**
     * @brief A 3D vector describing what direction is forward.
     */
    vec3 front;
    /**
     * @brief A 3D vector describing what direction is up.
     */
    vec3 up;
    /**
     * @brief A scalar representing the current amount of twist on the
     * vertical axis of movement.
     */
    float yaw;
    /**
     * @brief A scalar representing the pitch, or the lateral twist of the
     * camera on the x-axis. This measurement is perpendicular to the yaw.
     */
    float pitch;
    /**
     * @brief A value representing how fast the camera moves (in a
     * direction) on any button input.
     */
    float speed;
    /**
     * @brief A value representing how fast the camera's viewport moves on
     * any mouse input.
     */
    float sensitivity;
    /**
     * @brief A value representing how stretched out the items on the
     * screen are. This is stored in a float, but translated to a radian.
     */
    float fov;
    /**
     * @brief A value used to calculate the offset of any x-axis camera
     * movement.
     */
    float last_x;
    /**
     * @brief A value used to calculate the offset of any y-axis camera
     * movement.
     */
    float last_y;
} leto_camera_t;

/**
 * @brief An enumerator describing all the various directions that the
 * camera can move positionally in. They are all self-explanatory.
 */
typedef enum leto_movement_directions
{
    forward,
    backwards,
    left,
    right,
    up,
    down
} leto_movement_directions_t;

/**
 * CreateCamera
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Initialize all the values of a given camera object.
 *
 * @param camera The camera object in which we will store all the newly
 * initialized values.
 * @param fov The starting FOV of the camera.
 * @param speed The starting positional speed of the camera.
 * @param sensitivity The starting rotational speed of the camera.
 * @return bool -- True for success, false for failure.
 */
bool LetoCreateCamera(leto_camera_t *camera, float fov, float speed,
                      float sensitivity);

/**
 * SetCameraMatrix
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Set the given shader's "view" matrix to the given camera object.
 *
 * @param camera The camera object whose values we will use to assign the
 * view matrix.
 * @param shader The shader whose matrix we are trying to set.
 * @return void -- Nothing.
 */
void LetoSetCameraMatrix(leto_camera_t *camera, unsigned int shader);

/**
 * MoveCameraPosition
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Move the given camera in the given direction.
 *
 * @param camera The camera object whose matrices we should recalculate.
 * @param deltatime The time it took to calculate last frame, so we can
 * normalize the camera speed.
 * @param direction The direction in which to send the camera.
 * @return void -- Nothing.
 */
void LetoMoveCameraPosition(leto_camera_t *camera, float deltatime,
                            leto_movement_directions_t direction);

/**
 * MoveCameraOrientation
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Move the given camera rotationally to the new position.
 *
 * @param camera The camera object whose matrices we should recalculate.
 * @param x The newest mouse X position we have available.
 * @param y The newest mouse Y position we have available.
 * @return void -- Nothing.
 */
void LetoMoveCameraOrientation(leto_camera_t *camera, float x, float y);

#endif // LETO__CAMERA_H
