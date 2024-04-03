#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

out vec3 color;
out vec3 normal;
out vec3 position;

void main()
{
	gl_Position = mvpMatrix * vec4(aPos, 1.0);
	vec3 fragPos = vec3(modelMatrix * vec4(aPos, 1.0));

	
	position = fragPos;
	normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
	color=aColor;
}