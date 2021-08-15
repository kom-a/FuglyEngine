#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Fugly
{ 
	class Window
	{
	public:
		Window(int width, int height, const std::string& title);
		~Window();

		inline bool Closed() const { return glfwWindowShouldClose(m_Window); }

		void Update();


	private:
		void Init();

	private:
		GLFWwindow* m_Window;
		int m_Width, m_Height;
		std::string m_Title;
	};
}