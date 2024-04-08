#version 330 core

out vec4 FragColor;

uniform float near;
uniform float far;

void main()
{
	float depth = (2.0 * near * far) / ((far + near - (far - near) * gl_FragCoord.z) * far);
	FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
