#include <iostream>
#include <GLFW/glfw3.h>

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(1280, 720, "GLFW window", nullptr, nullptr);
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}