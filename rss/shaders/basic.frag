#version 460 core
out vec4 FragColor;
  
in vec4 vertex_color;

void main()
{
    FragColor = vertex_color;
}