#include "shader.h"

// code from https://learnopengl.com/Getting-started/Shaders

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
		std::string vertexCode;
		std::string fragmentCode;
		
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try 
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode   = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
}

void Shader::updateVertexShader(const char* newVertexPath)
{
	std::string newVertexCode;
	std::ifstream newVShaderFile;
	newVShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		newVShaderFile.open(newVertexPath);
		std::stringstream newVShaderStream;
		newVShaderStream << newVShaderFile.rdbuf();
		newVShaderFile.close();
		newVertexCode = newVShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		return;
	}

	const char* newVShaderCode = newVertexCode.c_str();

	if (vertex != 0) {
		glDeleteShader(vertex);
	}

	unsigned int newVertex;
	newVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(newVertex, 1, &newVShaderCode, NULL);
	glCompileShader(newVertex);
	checkCompileErrors(newVertex, "VERTEX");

	glDetachShader(ID, vertex);
	glAttachShader(ID, newVertex);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(newVertex);
	vertex = newVertex;
}

void Shader::updateFragmentShader(const char* newFragmentPath)
{
	std::string newFragmentCode;
	std::ifstream newFShaderFile;
	newFShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		newFShaderFile.open(newFragmentPath);
		std::stringstream newFShaderStream;
		newFShaderStream << newFShaderFile.rdbuf();
		newFShaderFile.close();
		newFragmentCode = newFShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		return;
	}

	const char* newFShaderCode = newFragmentCode.c_str();

	if (fragment != 0) {
		glDeleteShader(fragment);
	}

	unsigned int newFragment;
	newFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(newFragment, 1, &newFShaderCode, NULL);
	glCompileShader(newFragment);
	checkCompileErrors(newFragment, "FRAGMENT");

	glDetachShader(ID, fragment);
	glAttachShader(ID, newFragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(newFragment);
	fragment = newFragment;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setFloat(const std::string &name, float f) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), f);
}
