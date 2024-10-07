#version 460 core
layout(location = 0) in vec3 position;
// layout (location = 1) in vec3 normals;
// layout (location = 2) in vec2 texture_coordinates;

// out vec2 tc;
out vec3 pos;

uniform mat4 projection_matrix;
uniform mat4 camera_view;

uniform mat4 model;

void main()
{
    // tc = texture_coordinates;
    pos = position;
    gl_Position =
        projection_matrix * camera_view * model * vec4(position, 1.0);
}