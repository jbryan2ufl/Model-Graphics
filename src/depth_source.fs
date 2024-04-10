#version 330 core

out vec4 FragColor;

uniform float near;
uniform float far;

void main()
{
	// float depth = gl_FragCoord.z;
	float depth = (2.0 * near) / ((far + near - (far - near) * gl_FragCoord.z));
	FragColor = vec4(vec3(depth), 1.0);
}
