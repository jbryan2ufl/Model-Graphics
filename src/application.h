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
#include <glm/gtc/matrix_access.hpp>

#include <filesystem>
#include <algorithm>

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.h"
#include "object.h"
#include "camera.h"

struct Transformation
{
	std::string name{};
	glm::mat4 t{};
};

class Application
{
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

	void reload_data();

	void print_debug();

private:
	// settings
	int m_SCR_WIDTH{1440};
	int m_SCR_HEIGHT{1080};
	const float m_viewport_ratio{m_SCR_HEIGHT/static_cast<float>(m_SCR_WIDTH)};
	int m_VIEW_WIDTH{m_SCR_WIDTH*m_viewport_ratio};
	int m_VIEW_HEIGHT{m_SCR_HEIGHT};

	float m_nearPlane{0.1f};
	float m_farPlane{100.0f};

	Camera camera{};

	ImGuiIO* m_ioptr{};

	GLFWwindow* m_window{};

	Shader m_shader;
	Shader m_normalShader;
	Shader m_lightShader;

	// MVP matrix
	Transformation scale{"Scale", glm::mat4{1.0f}};
	Transformation rotate{"Rotate", glm::mat4{1.0f}};
	Transformation translate{"Translate", glm::mat4{1.0f}};
	Transformation view{"View", glm::mat4{1.0f}};
	Transformation projection{"Projection", glm::mat4{1.0f}};
	std::vector<Transformation*> mvpMatrixComponents{};
	glm::mat4 mvpMatrix{};
	glm::mat4 modelMatrix{};

	std::vector<std::string> obj_names{};

	unsigned int m_VAO;
	unsigned int m_colorVBO;
	unsigned int m_positionVBO;
	unsigned int m_EBO;
	unsigned int m_normalVBO;

	unsigned int m_lightVAO;
	unsigned int m_lightVBO;


	bool mouseFocus{};
	bool firstMouse{};
	float lastX{};
	float lastY{};

	float m_lastFrameTime{};
	float m_deltaTime{};

	bool depthVisualization{false};
	bool vsync{true};
	bool wireframe{false};
	bool useEBO{false};
	bool showNormals{false};

	int shaderSelection{0};

	Object* obj;
	Object* lightObj;

	float ambientStrength{0.0f};
	float diffuseStrength{1.0f};
	glm::vec3 lightPos{0.0f, 5.0f, 0.0f};
	glm::vec3 lightColor{1.0f, 1.0f, 1.0f};

	void draw();

	void process_input();
};

static void framebuffer_size_callback(GLFWwindow*, int, int);

static void key_callback(GLFWwindow*, int, int, int, int);

static void mouse_button_callback(GLFWwindow*, int, int, int);

static void cursor_position_callback(GLFWwindow* , double, double);

static void scroll_callback(GLFWwindow*, double, double);
