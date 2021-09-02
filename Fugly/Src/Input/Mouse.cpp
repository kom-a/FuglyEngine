#include "Mouse.h"

namespace Fugly
{

	double Mouse::s_MouseX;
	double Mouse::s_MouseY;
	bool Mouse::s_Buttons[MAX_BUTTONS];
	bool Mouse::s_MouseUsed = false;

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Mouse::s_MouseX = xpos;
		Mouse::s_MouseY = ypos;

		if (!Mouse::s_MouseUsed)
			Mouse::s_MouseUsed = true;
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Mouse::s_Buttons[button] = action != GLFW_RELEASE;
	}

}