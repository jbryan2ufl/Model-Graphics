#pragma once

#include "application.h"

void Application::draw()
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		m_shader.use();
		modelTransformation=glm::mat4{1.0f};
		for (auto& transformation : modelTransformationComponents)
		{
			modelTransformation*=(*transformation);
		}

		// modelTransformation=modelTransformationComponents[0].transform;
		m_shader.setMat4("modelTransformation", modelTransformation);
		glDrawArrays(GL_TRIANGLES, 0, obj.full_vertex_data.size());


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowBgAlpha(1.0f);
		glfwGetWindowSize(m_window, &m_SCR_WIDTH, &m_SCR_HEIGHT);
		ImGui::SetNextWindowPos(ImVec2{m_SCR_WIDTH*3/4,0});
		ImGui::SetNextWindowSize(ImVec2{m_SCR_WIDTH*1/4,m_SCR_HEIGHT});
		if (ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration))
		{
			// if (ImGui::Button("Add Transformation"))
			// {
			// 	modelTransformationComponents.push_back(Transformation{"t"+std::to_string(modelTransformationComponents.size()), glm::scale(glm::mat4{1.0f}, glm::vec3{0.5f})});
			// }

			// ImGui::SameLine();
			// if (ImGui::Button("Remove Transformation"))
			// {
			// 	modelTransformationComponents.pop_back();
			// }

			for (auto& transformation : modelTransformationComponents)
			{
				// if (ImGui::CollapsingHeader())
				// {
					if (ImGui::BeginTable("", 4))
					{
						for (int i{}; i < 4; i++)
						{
							ImGui::TableNextRow();
							for (int j{}; j < 4; j++)
							{
								ImGui::TableSetColumnIndex(j);
								ImGui::Text(std::to_string((*transformation)[j][i]).c_str());
								// ImGui::SliderFloat(("##float"+std::to_string(i)+std::to_string(j)).c_str(), &transformation.transform[i][j], 0.0f, 1.0f);
							}
						}
						ImGui::EndTable();
					}
				// }
			}
		}
		ImGui::End();
		ImGui::Render();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_window);
}

Application::Application()
{}

void Application::init()
{

	// glfw initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// glfw window creation
	m_window = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "Model Transformations", NULL, NULL);

	// check if window is created
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}

	// glfw callback configuration
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	// glfw mouse capture
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	// imgui configuration
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	m_ioptr=&io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();
	

	// start glew
	glewInit();

	glfwSwapInterval(1);

	m_shader = Shader("src/source.vs", "src/source.fs");

	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	std::cout << obj.vertex_count << '\n';

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, obj.full_vertex_data.size()*sizeof(glm::vec3), obj.full_vertex_data.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glfwSetWindowUserPointer(m_window, this);

	glViewport(0, 0, m_SCR_WIDTH*3/4, m_SCR_HEIGHT);
	modelTransformationComponents.push_back(&translate);
	modelTransformationComponents.push_back(&scale);
	modelTransformationComponents.push_back(&rotate);
}


void Application::process_framebuffer_size(int width, int height)
{
	glViewport(0, 0, width*3/4, height);
}

void Application::process_key(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(m_window, true);
			close();
		}
	}
}

void Application::process_mouse_button(int button, int action, int mods)
{
	if (!m_ioptr->WantCaptureMouse)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			firstMouse=true;
		}
	}
}

void Application::process_cursor_position(double xposIn, double yposIn)
{
	// if (!m_ioptr->WantCaptureMouse)
	// {
	// 	float xpos=static_cast<float>(xposIn);
	// 	float ypos=static_cast<float>(yposIn);

	// 	if (firstMouse)
	// 	{
	// 		lastX=xpos;
	// 		lastY=ypos;
	// 		firstMouse=false;
	// 	}

	// 	float xoffset = xpos-lastX;
	// 	float yoffset = ypos-lastY;

	// 	lastX=xpos;
	// 	lastY=ypos;

	// 	rotate=glm::rotate(rotate,xoffset*0.01f,glm::vec3{0.0f, 1.0f, 0.0f});
	// 	rotate=glm::rotate(rotate,yoffset*0.01f,glm::vec3{1.0f, 0.0f, 0.0f});
	// }
}

void Application::process_scroll(double xoffset, double yoffset)
{
	scale=glm::scale(scale, glm::vec3{1+yoffset*0.1f});
}

void Application::process_input()
{
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		// rotate = glm::rotate(rotate, -0.01f, glm::vec3{0.0f, 0.0f, -1.0f});
		translate = glm::translate(translate, glm::vec3{-0.01f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		// rotate = glm::rotate(rotate, 0.01f, glm::vec3{0.0f, 0.0f, -1.0f});
		translate = glm::translate(translate, glm::vec3{0.01f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		translate = glm::translate(translate, glm::vec3{0.0f, 0.01f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		translate = glm::translate(translate, glm::vec3{0.0f, -0.01f, 0.0f});
	}

	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotate = glm::rotate(rotate, 0.01f, glm::vec3{0.0f, 0.0f, 1.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotate = glm::rotate(rotate, -0.01f, glm::vec3{0.0f, 0.0f, 1.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		rotate = glm::rotate(rotate, 0.01f, glm::vec3{1.0f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		rotate = glm::rotate(rotate, -0.01f, glm::vec3{1.0f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		rotate = glm::rotate(rotate, -0.01f, glm::vec3{0.0f, 1.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		rotate = glm::rotate(rotate, 0.01f, glm::vec3{0.0f, 1.0f, 0.0f});
	}

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		firstMouse=true;
	}
}


void Application::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		process_input();
		draw();
		glfwPollEvents();
	}
}

void Application::close()
{
	glfwTerminate();
	exit(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_framebuffer_size(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_key(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_mouse_button(button, action, mods);
}

void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_cursor_position(xposIn, yposIn);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_scroll(xoffset, yoffset);
}
