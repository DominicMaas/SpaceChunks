#version 303 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_position = vec4(xPos.x, aPos.y, aPos.z, 1.0);
}