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
#include "Graphics/Primitives/Cube.h"
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
    
	glClearColor(0.7f, 0.8f, 0.9f, 1.0f);
    
	float lastTime = glfwGetTime();
	float unprocessedTime = 0;
	int fps = 0;
    
	//Model Sponza("Res/Models/sponza/sponza.obj");
	Model Backpack("Res/Models/Backpack.obj");
	Cube cube(glm::vec3(0.1));

	Shader shader("Res/Shaders/TestVertex.glsl", "Res/Shaders/TestFragment.glsl");
	Shader singleColorShader("Res/Shaders/TestVertex.glsl", "Res/Shaders/FragmentShader.glsl");
	glm::mat4 testModel(1.0f);
	glm::mat4 testProjection = glm::perspective(glm::radians(45.0f), window.Aspect(), 0.1f, 250.0f);
    
	Camera camera(glm::vec3(0, 2, 10));
	
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LESS);
	
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
        
		glEnable(GL_DEPTH_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		testModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		shader.Bind();

		shader.SetMatrix4("u_Projection", testProjection);
		shader.SetMatrix4("u_View", camera.GetViewMatrix());
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		shader.SetUniform3f("u_CameraPos", camera.GetPosition());
        
		LOG_GL_ERRORS();
        
		shader.SetUniform1i("diffuseSampler", 0);
		shader.SetUniform1i("specularSampler", 1);
        
		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0));
		testModel = glm::scale(testModel, glm::vec3(25, 0.25, 25));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		cube.Render();

		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.75, 0));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Backpack.Render();

		camera.Update(deltaTime);
		window.Update();
		fps++;
	}
    
	return 0;
}