#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

void main() {
	gl_Position = mvpMatrix * vec4(aPos, 1.0);
	normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
}