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
#include "Graphics/GUI/Engine/GUI.h"
#include "Utils/Log.h"
#include "Input/Input.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

int main()
{
	using namespace Fugly;

	Window window(1280, 720, "Fugly");

	//Renderer renderer;

	glClearColor(0.4f, 0.6f, 0.8f, 1.0f);

	float lastTime = glfwGetTime();
	float unprocessedTime = 0;
	int fps = 0;

	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), window.Aspect(), 0.1f, 100.0f);

	Model Sponza("Res/Models/sponza/sponza.obj");
	Model Backpack("Res/Models/Backpack.obj");

	Shader shader("Res/Shaders/TestVertex.glsl", "Res/Shaders/TestFragment.glsl");
	glm::mat4 testModel(1.0f);
	glm::mat4 testProjection = glm::perspective(glm::radians(45.0f), window.Aspect(), 0.01f, 100000.0f);

	Camera camera(glm::vec3(0, 0, 10));
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GUI::SceneWindow sceneWindow(&window);

	FrameBuffer FBO(sceneWindow.GetTextureID());

	while (!window.Closed())
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		unprocessedTime += deltaTime;

		if (unprocessedTime >= 1)
		{
			unprocessedTime = 0;
			LOG_TRACE("FPS: {0}({1}ms)", fps, 1.0f / fps * 1000);
			fps = 0;
		}

		if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			window.Close();
		
		shader.Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		testModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		shader.SetMatrix4("u_Projection", testProjection);
		shader.SetMatrix4("u_View", camera.GetViewMatrix());
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		shader.SetUniform3f("u_CameraPos", camera.GetPosition());

		LOG_GL_ERRORS();

#if 0
		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(-7, 1, 0));
		testModel = glm::rotate(testModel, (float)glfwGetTime() * 1.5f, glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(1.0f));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Teapot.Render();
		
		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		testModel = glm::rotate(testModel, (float)glfwGetTime() * 1.0f, glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(25.0f));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Bunny.Render();

		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(7, 0, 0));
		testModel = glm::rotate(testModel, (float)glfwGetTime() * 0.5f, glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(25.0f));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Dragon.Render();
#endif
		
		shader.SetUniform1i("diffuseSampler", 0);
		shader.SetUniform1i("specularSampler", 1);

		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		testModel = glm::rotate(testModel, 0.0f, glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(0.05f));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Sponza.Render();

		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.55f, 0));
		testModel = glm::rotate(testModel, float(glfwGetTime()), glm::vec3(0, 1, 0));
		testModel = glm::scale(testModel, glm::vec3(1.0f));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Backpack.Render();

		camera.Update(deltaTime);
		window.Update();
		fps++;
	}

	return 0;
}