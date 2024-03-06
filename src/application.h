#pragma once

// stl includes
#include <iostream>
#include <vector>
#include <string>

// render includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.h"
#include "object.h"

class Application
{
public:
	// settings
	int m_SCR_WIDTH = 1280;
	int m_SCR_HEIGHT = 720;
	int m_VIEW_WIDTH = 1280*3/4;
	int m_VIEW_HEIGHT = 1280;

	ImGuiIO* m_ioptr{};

	GLFWwindow* m_window{};

	Shader m_shader{};

	glm::mat4 modelTransformation{1.0f};

	glm::mat4 scale{1.0f};
	glm::mat4 rotate{1.0f};
	glm::mat4 translate{1.0f};
	std::vector<glm::mat4*> modelTransformationComponents{};

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	bool firstMouse{};
	float lastX{};
	float lastY{};

	Object obj{};

	void draw();

	void process_input();

	public:

	Application();

	void init();

	void process_framebuffer_size(int, int);

	void process_key(int, int, int, int);

	void process_mouse_button(int, int, int);

	void process_cursor_position(double, double);

	void process_scroll(double, double);


	void run();

	void close();
};

static void framebuffer_size_callback(GLFWwindow*, int, int);

static void key_callback(GLFWwindow*, int, int, int, int);

static void mouse_button_callback(GLFWwindow*, int, int, int);

static void cursor_position_callback(GLFWwindow* , double, double);

static void scroll_callback(GLFWwindow*, double, double);
