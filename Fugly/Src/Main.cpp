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
#include "Graphics/Model.h"
#include "Utils/Log.h"
#include "Input/Input.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

std::vector<float> Positions;
std::vector<unsigned int> Indices;

int main()
{
	using namespace Fugly;

	Window window(1280, 720, "Fugly");

	Shader shader("Res/VertexShader.glsl", "Res/FragmentShader.glsl");
	shader.Bind();

	Renderer renderer;

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

	float lastTime = glfwGetTime();
	float unprocessedTime = 0;
	int fps = 0;

	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), window.Aspect(), 0.1f, 100.0f);

	float speed = 2.0f;

	Sprite sprite(glm::vec3(0, 0, -50.0f), glm::vec3(50, 50, 0.0f), glm::vec4(0.8f, 0.5f, 0.3f, 1.0f));

	Model Teapot("Res/Teapot.obj");
	Model Bunny("Res/Bunny.obj");
	Model Dragon("Res/Dragon.ply");

	Model Sponza("Res/Sponza.gltf");

	float cubeVertices[] = {
		-0.5f,  0.5f, 0.5f,	
		 0.5f,  0.5f, 0.5f,	
		-0.5f, -0.5f, 0.5f,	
		 0.5f, -0.5f, 0.5f,	

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
	};

	unsigned int cubeIndices[] = {
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
	glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(float), &Positions[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(0));
	glEnableVertexAttribArray(0);

	unsigned IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	
	Texture texture("Res/container.jpg", 0);

	Shader testShader("Res/TestVertex.glsl", "Res/TestFragment.glsl");
	glm::mat4 testModel(1.0f);
	glm::mat4 testProjection = glm::perspective(glm::radians(45.0f), window.Aspect(), 0.01f, 1000.0f);

	Camera camera(glm::vec3(0, 0, 15), glm::vec3(0, 0, -1));
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	renderer.Begin();

	renderer.Submit(Teapot);
	renderer.Submit(Sponza);

	renderer.End();

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

		

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		texture.Bind(0);
		testShader.Bind();

		testModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.025f));

		testShader.SetMatrix4("u_Projection", testProjection);
		testShader.SetMatrix4("u_View", camera.GetViewMatrix());
		testShader.SetMatrix4("u_Model", testModel);
		testShader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));

		renderer.Flush();

#if 0
		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(-7, 1, 0));
		testModel = glm::rotate(testModel, (float)glfwGetTime() * 1.5f, glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(1.0f));
		testShader.SetMatrix4("u_Model", testModel);
		testShader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Teapot.Render();
		
		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		testModel = glm::rotate(testModel, (float)glfwGetTime() * 1.0f, glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(25.0f));
		testShader.SetMatrix4("u_Model", testModel);
		testShader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Bunny.Render();

		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(7, 0, 0));
		testModel = glm::rotate(testModel, (float)glfwGetTime() * 0.5f, glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(25.0f));
		testShader.SetMatrix4("u_Model", testModel);
		testShader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Dragon.Render();

		testModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.05));
		testShader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		testShader.SetMatrix4("u_Model", testModel);
		Sponza.Render();
	
#endif

		camera.Update(deltaTime);
		window.Update();
		fps++;
	}

	return 0;
}