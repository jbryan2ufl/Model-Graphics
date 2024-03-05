#pragma once

// stl includes
#include <iostream>
#include <vector>
// #include <cmath>
// #include <Windows.h>
// #include <cstdlib>
// #include <ctime>
// #include <random>
//#include <limits>

// render includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.h"

class Application
{
	public:
	// settings
	int m_SCR_WIDTH = 1280;
	int m_SCR_HEIGHT = 720;

	ImGuiIO* m_ioptr{};

	GLFWwindow* m_window{};

	Shader m_shader{};

	std::vector<float> m_vertices;
	int m_vertexCount{};

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	void draw();

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	void processInput(GLFWwindow*);

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
