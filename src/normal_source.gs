#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in vec3 normal[];

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 mvpMatrix;

void generateLine(int i)
{
	gl_Position = gl_in[i].gl_Position;
	EmitVertex();

	gl_Position = gl_in[i].gl_Position + (mvpMatrix * vec4(2*normalize(normal[i]), 1.0));
	EmitVertex();
	EndPrimitive();
}

void main()
{
	// generateLine(0);
	// generateLine(1);
	generateLine(2);
}
