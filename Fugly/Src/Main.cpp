#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Graphics/Window.h"

int main()
{
	using namespace Fugly;

	Window window(1280, 720, "Fugly");

	while (!window.Closed())
	{
		window.Update();
	}

	return 0;
}