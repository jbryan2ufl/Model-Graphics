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
	uint32_t ID{};

	Shader(){};
	Shader(const char*, const char*);
	void use();
	void checkCompileErrors(unsigned int, std::string);
	void setMat4(const std::string &name, const glm::mat4 &mat)const;

};