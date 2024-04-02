#version 330 core

out vec4 FragColor;

uniform float near;
uniform float far;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	float depth = gl_FragCoord.z;
	float linearizedDepth = LinearizeDepth(depth) / far;
	FragColor = vec4(vec3(linearizedDepth), 1.0);
}
