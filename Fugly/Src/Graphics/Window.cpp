#include "Window.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Utils/Log.h"
#include "../Input/Input.h"

namespace Fugly
{
	Window::Window(int width, int height, const std::string& title)
		:	m_Width(width),
			m_Height(height),
			m_Title(title)
	{
		Init();
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::Init()
	{
		if (!Log::Init())
		{
			printf("Failed to initiazile logging system.");
			return;
		}

		if (glfwInit() != GLFW_TRUE)
		{
			LOG_CRITICAL("Failed to initialize GLFW.");
			return;
		}

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		if (!m_Window)
		{
			LOG_CRITICAL("Failed to create GLFW window.");
			return;
		}
		glfwMakeContextCurrent(m_Window);

		if (glewInit() != GLEW_OK)
		{
			LOG_CRITICAL("Failed to initialize GLEW.");
			return;
		}

		glfwSwapInterval(0);

		glfwSetCursorPosCallback(m_Window, cursor_position_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		InitImGui();

		LOG_INFO("Fugly engine has been initialized successfully!");
		LOG_INFO("OpenGL version: {0}", glGetString(GL_VERSION));
	}

	void Window::InitImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_Width = width;
		win->m_Height = height;
		glViewport(0, 0, width, height);
	}
}

