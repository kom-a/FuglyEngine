#pragma once

#define MAX_BUTTONS 16

#include <GLFW/glfw3.h>

namespace Fugly
{

	class Mouse
	{
	public:
		inline static double GetX() { return s_MouseX; }
		inline static double GetY() { return s_MouseY; }
		inline static bool IsButtonPressed(int button) { return s_Buttons[button]; }

	private:
		static bool s_Buttons[MAX_BUTTONS];
		static double s_MouseX, s_MouseY;

		friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	private:
		friend class Camera;
	public:
		static bool s_MouseUsed;
	};

}