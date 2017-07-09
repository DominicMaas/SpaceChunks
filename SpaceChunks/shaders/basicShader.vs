#version 400 core

layout(location = 0) in vec4 coord;
uniform mat4 mvp;
varying vec4 texcoord;

void main()
{
	texcoord = coord;
    gl_Position = mvp * vec4(coord.xyz, 1.0); 
}