#include "Window.h"

#include "../Utils/Log.h"

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

		LOG_INFO("Fugly engine has been initialized successfully!");
		LOG_INFO("OpenGL version: {0}", glGetString(GL_VERSION));
	}

	void Window::Update()
	{
		LOG_GL_ERRORS();
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
}

