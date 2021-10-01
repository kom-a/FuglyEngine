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
#include "Graphics/Primitives/Plane.h"
#include "Utils/Log.h"
#include "Input/Input.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui.h>

#include <vector>

#include <stb/stb_image.h>

int main()
{
	using namespace Fugly;
    
	Window window(1280, 720, "Fugly");

	float lastTime = glfwGetTime();
	float unprocessedTime = 0;
	int fps = 0;
    
	FrameBuffer FBO(window.GetWidth(), window.GetHeight());

	Model Sponza("Res/Models/sponza/sponza.obj");
	Model Backpack("Res/Models/Backpack.obj");

	Shader shader("Res/Shaders/TestVertex.glsl", "Res/Shaders/TestFragment.glsl");
	
	Camera camera(glm::vec3(0, 2, 10));
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), window.Aspect(), 0.01f, 500.0f);
	glm::mat4 model = glm::mat4(1.0f);

	float fov = 45.0f;

	glm::vec2 lastMouse = { Mouse::GetX(), Mouse::GetY() };

	while (!window.Closed())
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
        
		unprocessedTime += deltaTime;
        
		// Print FPS
		if (unprocessedTime >= 1)
		{
			unprocessedTime = 0;
			LOG_TRACE("FPS: {0}({1}ms)", fps, 1.0f / fps * 1000);
			fps = 0;
		}
        
		if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			window.Close();

		if (Keyboard::IsKeyPressed(GLFW_KEY_UP))
		{
			fov -= 100 * deltaTime;
		}
		else if (Keyboard::IsKeyPressed(GLFW_KEY_DOWN))
		{
			fov += 100 * deltaTime;
		}
		
		FBO.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(glm::radians(fov), window.Aspect(), 0.1f, 500.0f);

		shader.Bind();
		shader.SetUniform3f("u_CameraPos", camera.GetPosition());
		shader.SetMatrix4("u_View", camera.GetViewMatrix());
		shader.SetMatrix4("u_Projection", projection);

		model = glm::translate(glm::mat4(1.0f), Backpack.GetPosition());
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(1.0f));
		shader.SetMatrix4("u_Model", model);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(model)));
		Backpack.Render();


		model = glm::translate(glm::mat4(1.0f), glm::vec3(0));
		model = glm::scale(model, glm::vec3(0.05f));
		shader.SetMatrix4("u_Model", model);
		shader.SetMatrix3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
		Sponza.Render();
		
		FBO.Unbind();
		
		// ImGUI render
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			
			ImGui::DockSpaceOverViewport();
			ImGui::ShowDemoWindow();
			ImGui::ShowMetricsWindow();
			ImGui::Begin("Scene");
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddImage(
				(void*)FBO.GetColorBuffer(), ImVec2(pos.x, pos.y + +ImGui::GetWindowSize().y - 35),
				ImVec2(pos.x + ImGui::GetWindowSize().x - 15, pos.y));

			glm::vec2 sceneWindowPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			glm::vec2 sceneWindowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

			ImGui::End();
			glm::vec2 mouse = { Mouse::GetX(), Mouse::GetY() };

			ImGui::Begin("Transform");

			// TODO: Add transform ImGui window
			ImGui::DragFloat3("Position", &Backpack.GetPosition().x, 0.01f);

			ImGui::End();


			if (Mouse::IsButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) &&
				lastMouse.x > sceneWindowPos.x && lastMouse.x < sceneWindowPos.x + sceneWindowSize.x &&
				lastMouse.y > sceneWindowPos.y && lastMouse.y < sceneWindowPos.y + sceneWindowSize.y)
			{
				window.SetMouseEnabled(false);
				camera.Update(deltaTime);
			}
			else
			{
				window.SetMouseEnabled(true);
				camera.ResetFirstMouse();
				lastMouse = { Mouse::GetX(), Mouse::GetY() };
			}
		}
		
		
		window.Update();
		fps++;
	}
    
	return 0;
}