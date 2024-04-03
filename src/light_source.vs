#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 vpMatrix;
uniform vec3 lightPos;

void main()
{
	gl_Position = vpMatrix * vec4(aPos*0.1+lightPos, 1.0);
}