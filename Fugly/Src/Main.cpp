#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Graphics/Window.h"
#include "Graphics/Buffers/Buffers.h"
#include "Graphics/Shader.h"
#include "Graphics/Renderer.h"
#include "Graphics/Sprite.h"
#include "Utils/Log.h"

int main()
{
	using namespace Fugly;

	Window window(1280, 720, "Fugly");

	float vertices[]
	{
		-0.5f, -0.5f, 0.0f, 
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	Shader shader("Res/VertexShader.glsl", "Res/FragmentShader.glsl");
	shader.Bind();

	Renderer renderer;

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

	double lastTime = glfwGetTime();
	double unprocessedTime = 0;
	int fps = 0;

	while (!window.Closed())
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		unprocessedTime += deltaTime;

		if (unprocessedTime >= 1)
		{
			unprocessedTime = 0;
			LOG_TRACE("FPS: {0} ", fps);
			fps = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		renderer.Begin();

		float step = (glm::sin(glfwGetTime()) + 1) / 2.0f;

		for (float i = -1; i <= 1.; i += 0.1)
		{
			for (float j = -1; j <= 1.; j += 0.1)
			{
				renderer.Submit(Sprite(glm::vec3(i, j, 0), glm::vec3(0.1, 0.1, 0.0f), glm::vec4((i + 1) / 2.0f, (j + 1) / 2.0f, 0.5f, 1.0f)));
			}
		}

		renderer.End();

		renderer.Flush();

		window.Update();
		fps++;
	}

	return 0;
}