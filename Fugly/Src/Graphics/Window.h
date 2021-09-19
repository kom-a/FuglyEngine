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

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline bool Closed() const { return glfwWindowShouldClose(m_Window); }
		inline float Aspect() const { return (float)m_Width / m_Height; }

		void SetMouseEnabled(bool enabled);

		friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		void Update();

		inline void Close() { glfwSetWindowShouldClose(m_Window, true); }


	private:
		void Init();
		void InitImGui();

	private:
		GLFWwindow* m_Window;
		int m_Width, m_Height;
		std::string m_Title;
	};
}