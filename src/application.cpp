#pragma once

#define EBO

#include "application.h"

void Application::draw()
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		m_shader.use();
		modelTransformation=glm::mat4{1.0f};
		for (auto& transformation : modelTransformationComponents)
		{
			modelTransformation*=transformation->t;
		}

		// modelTransformation=modelTransformationComponents[0].transform;
		m_shader.setMat4("modelTransformation", modelTransformation);
		#ifdef EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glDrawElements(GL_TRIANGLES, obj->full_index_data.size()*sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		#else
			glDrawArrays(GL_TRIANGLES, 0, obj->full_vertex_data.size()*sizeof(glm::vec3));
		#endif

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowBgAlpha(1.0f);
		glfwGetWindowSize(m_window, &m_SCR_WIDTH, &m_SCR_HEIGHT);
		ImGui::SetNextWindowPos(ImVec2{m_SCR_WIDTH*3/4,0});
		ImGui::SetNextWindowSize(ImVec2{m_SCR_WIDTH*1/4,m_SCR_HEIGHT});
		if (ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration))
		{
			for (auto& transformation : modelTransformationComponents)
			{
				ImGui::Text(transformation->name.c_str());
				if (ImGui::BeginTable("", 4))
				{
					for (int i{}; i < 4; i++)
					{
						ImGui::TableNextRow();
						for (int j{}; j < 4; j++)
						{
							ImGui::TableSetColumnIndex(j);
							ImGui::Text(std::to_string(transformation->t[j][i]).c_str());
							// ImGui::SliderFloat(("##float"+std::to_string(i)+std::to_string(j)).c_str(), &transformation.transform[i][j], 0.0f, 1.0f);
						}
					}
					ImGui::EndTable();
				}
				ImGui::NewLine();
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

	glEnable(GL_DEPTH_TEST);


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

	glGenBuffers(1, &m_ColorVBO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	obj = new Object();
	// std::cout << obj->vertex_count << '\n';
	// std::cout << obj->full_vertex_data.size()*sizeof(glm::vec3)*2 << '\n';
	// std::cout << sizeof(obj->full_vertex_data)*obj->full_vertex_data.size() << '\n';

	#ifdef EBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, obj->vertex_data.size()*sizeof(glm::vec3), obj->vertex_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_ColorVBO);
	glBufferData(GL_ARRAY_BUFFER, obj->color_data.size()*sizeof(glm::vec3), obj->color_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->full_index_data.size()*sizeof(unsigned int), obj->full_index_data.data(), GL_STATIC_DRAW);

	#else
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, obj->full_vertex_data.size()*sizeof(glm::vec3), obj->full_vertex_data.data(), GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, obj->full_vertex_data.size()*sizeof(glm::vec3)*2, obj->full_vertex_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_ColorVBO);
	glBufferData(GL_ARRAY_BUFFER, obj->full_color_data.size()*sizeof(glm::vec3), obj->full_color_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)0);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	// glEnableVertexAttribArray(1);
	#endif

	glfwSetWindowUserPointer(m_window, this);

	glViewport(0, 0, m_SCR_WIDTH*3/4, m_SCR_HEIGHT);
	modelTransformationComponents.push_back(&translate);
	modelTransformationComponents.push_back(&rotate);
	modelTransformationComponents.push_back(&scale);
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
	}
}

void Application::process_cursor_position(double xposIn, double yposIn)
{
	if (!m_ioptr->WantCaptureMouse)
	{
		float xpos=static_cast<float>(xposIn);
		float ypos=static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX=xpos;
			lastY=ypos;
			firstMouse=false;
		}

		float xoffset = xpos-lastX;
		float yoffset = ypos-lastY;

		lastX=xpos;
		lastY=ypos;
		rotate.t=glm::rotate(rotate.t,-xoffset*0.01f,glm::vec3{glm::row(rotate.t, 1)});
		rotate.t=glm::rotate(rotate.t,-yoffset*0.01f,glm::vec3{glm::row(rotate.t, 0)});
		// rotate.t=glm::rotate.t(rotate.t,-xoffset*0.01f,glm::vec3{0.0f, 1.0f, 0.0f});
		// rotate.t=glm::rotate.t(rotate.t,-yoffset*0.01f,glm::vec3{1.0f, 0.0f, 0.0f});
	}
}

void Application::process_scroll(double xoffset, double yoffset)
{
	scale.t=glm::scale(scale.t, glm::vec3{1+yoffset*0.1f});
}

void Application::process_input()
{
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		// rotate.t = glm::rotate.t(rotate.t, -0.01f, glm::vec3{0.0f, 0.0f, -1.0f});
		translate.t = glm::translate(translate.t, glm::vec3{-0.01f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		// rotate.t = glm::rotate.t(rotate.t, 0.01f, glm::vec3{0.0f, 0.0f, -1.0f});
		translate.t = glm::translate(translate.t, glm::vec3{0.01f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		translate.t = glm::translate(translate.t, glm::vec3{0.0f, 0.01f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		translate.t = glm::translate(translate.t, glm::vec3{0.0f, -0.01f, 0.0f});
	}

	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		// rotate.t = glm::rotate.t(rotate.t, 0.01f, glm::vec3{0.0f, 0.0f, 1.0f});
		rotate.t = glm::rotate(rotate.t, 0.01f, glm::vec3{glm::row(rotate.t, 2)});
	}
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		// rotate.t = glm::rotate.t(rotate.t, -0.01f, glm::vec3{0.0f, 0.0f, 1.0f});
		rotate.t = glm::rotate(rotate.t, -0.01f, glm::vec3{glm::row(rotate.t, 2)});
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		rotate.t = glm::rotate(rotate.t, 0.01f, glm::vec3{glm::row(rotate.t, 0)});
		// rotate.t = glm::rotate.t(rotate.t, 0.01f, glm::vec3{1.0f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		rotate.t = glm::rotate(rotate.t, -0.01f, glm::vec3{glm::row(rotate.t, 0)});
		// rotate.t = glm::rotate.t(rotate.t, -0.01f, glm::vec3{1.0f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		rotate.t = glm::rotate(rotate.t, 0.01f, glm::vec3{glm::row(rotate.t, 1)});
		// rotate.t = glm::rotate.t(rotate.t, -0.01f, glm::vec3{0.0f, 1.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		rotate.t = glm::rotate(rotate.t, -0.01f, glm::vec3{glm::row(rotate.t, 1)});
		// rotate.t = glm::rotate.t(rotate.t, 0.01f, glm::vec3{0.0f, 1.0f, 0.0f});
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
	delete obj;
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
