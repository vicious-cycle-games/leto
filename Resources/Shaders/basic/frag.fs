#version 460 core
layout(location = 0) out vec4 fragmentColor;

// in vec2 tc;
in vec3 pos;

// uniform sampler2D texture_diffuse1;

void main()
{
    fragmentColor = vec4(pos.x, pos.y, pos.z, 1.0);
    // FragColor = texture(texture_diffuse1, tc);
}