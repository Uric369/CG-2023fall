#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in GLfloat colorIndice;

uniform vec4 colors[10];

out vec3 ourColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    ourColor = colors[int(colorIndice)];
}