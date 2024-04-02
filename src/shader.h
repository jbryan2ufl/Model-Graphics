#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID{};
	unsigned int vertex{};
	unsigned int fragment{};

	Shader(){};
	Shader(const char*, const char*);
	void updateVertexShader(const char* newVertexPath);
	void updateFragmentShader(const char* newFragmentPath);
	void use();
	void checkCompileErrors(unsigned int, std::string);
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setFloat(const std::string &name, float f) const;
};