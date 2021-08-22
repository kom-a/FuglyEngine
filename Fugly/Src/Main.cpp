#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/Window.h"
#include "Graphics/Buffers/Buffers.h"
#include "Graphics/Shader.h"
#include "Graphics/Renderer.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "Utils/Log.h"
#include "Input/Input.h"

int main()
{
	using namespace Fugly;

	Window window(1280, 720, "Fugly");

	Shader shader("Res/VertexShader.glsl", "Res/FragmentShader.glsl");
	shader.Bind();

	Renderer renderer;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	float lastTime = glfwGetTime();
	float unprocessedTime = 0;
	int fps = 0;

	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), window.Aspect(), 0.1f, 100.0f);

	float speed = 2.0f;

	Sprite sprite(glm::vec3(0, 0, -50.0f), glm::vec3(50, 50, 0.0f), glm::vec4(0.8f, 0.5f, 0.3f, 1.0f));

	float cubeVertices[] = {
		-0.5f,  0.5f, 0.5f,		0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,		1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
	};

	short indices[] = {
		0, 1, 2, 1, 2, 3,
		4, 5, 6, 5, 6, 7,
		0, 2, 4, 2, 4, 6,
		1, 3, 5, 3, 5, 7,
		2, 3, 6, 3, 6, 7,
		0, 1, 4, 1, 4, 5,
	};

	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * 4, (const void*)(0 * 4));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * 4, (const void*)(3 * 4));
	glEnableVertexAttribArray(1);

	unsigned IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Texture texture("Res/container.jpg", 0);

	Shader testShader("Res/TestVertex.glsl", "Res/TestFragment.glsl");
	glm::mat4 testModel(1.0f);
	glm::mat4 testProjection = glm::perspective(glm::radians(45.0f), window.Aspect(), 0.01f, 100.0f);

	Camera camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1));
	
	glEnable(GL_DEPTH_TEST);

	while (!window.Closed())
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		unprocessedTime += deltaTime;

		if (unprocessedTime >= 1)
		{
			unprocessedTime = 0;
			LOG_TRACE("FPS: {0} ", fps);
			fps = 0;
		}

		if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			window.Close();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		/*shader.Bind();
		shader.SetMatrix4("u_Model", model);
		shader.SetMatrix4("u_View", view);
		shader.SetMatrix4("u_Projection", projection);

		renderer.Begin();
		renderer.Submit(sprite);
		renderer.End();

		renderer.Flush();*/

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		texture.Bind(0);
		testShader.Bind();
		testShader.SetMatrix4("u_MVP", testProjection * camera.GetViewMatrix() * testModel);
		
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		camera.Update(deltaTime);
		window.Update();
		fps++;
	}

	return 0;
}