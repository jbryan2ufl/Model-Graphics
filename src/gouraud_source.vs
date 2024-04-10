#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

uniform vec3 viewPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

out vec3 color;

void main()
{
	gl_Position = mvpMatrix * vec4(aPos, 1.0);
	vec3 fragPos = vec3(modelMatrix * vec4(aPos, 1.0));
	
	vec3 normal = mat3(transpose(inverse(modelMatrix))) * aNormal;

	vec3 norm = normalize(normal);

	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * aColor;
	color = result;
}