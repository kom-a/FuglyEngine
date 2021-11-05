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


static unsigned int LoadCubeMap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);

	int width, height, channels;
	for (int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
		int format = channels == 4 ? GL_RGBA : GL_RGB;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			Fugly::LOG_ERROR("Failed to load cubemap texture: {0}", faces[i]);
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

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
	Model Planet("Res/Models/Planet/planet.obj");
	Model Rock("Res/Models/Rock/rock.obj");

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

	float fov = 60.0f;

	std::vector<std::string> faces
	{
		"Res/Models/skybox/right.png",
		"Res/Models/skybox/left.png",
		"Res/Models/skybox/top.png",
		"Res/Models/skybox/bottom.png",
		"Res/Models/skybox/front.png",
		"Res/Models/skybox/back.png"
	};

	unsigned int cubemapTexture = LoadCubeMap(faces);

	float cubeMapVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	VertexArray cubeMapVAO;
	cubeMapVAO.Bind();
	unsigned int cubeMapVBO;
	glGenBuffers(1, &cubeMapVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMapVertices), cubeMapVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	Shader skyboxShader("Res/Shaders/SkyboxVertex.glsl", "Res/Shaders/SkyboxFragment.glsl");

	glm::vec2 lastMouse = { Mouse::GetX(), Mouse::GetY() };

	Sponza.Scale *= 0.005f;

	Sponza.Position += glm::vec3(50, 0, 0);

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

		// ============================ Skybox =================================================
		glDisable(GL_DEPTH_TEST);

		skyboxShader.Bind();
		skyboxShader.SetMatrix4("u_View", glm::mat4(glm::mat3(camera.GetViewMatrix())));
		skyboxShader.SetMatrix4("u_Projection", projection);
		cubeMapVAO.Bind();
		glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glEnable(GL_DEPTH_TEST);

		// =========================== End of drawing skybox ===================================

		shader.Bind();
		shader.SetUniform3f("u_CameraPos", camera.GetPosition());
		shader.SetMatrix4("u_View", camera.GetViewMatrix());
		shader.SetMatrix4("u_Projection", projection);

		model = glm::translate(glm::mat4(1.0f), Backpack.GetPosition());
		model = glm::rotate(model, Backpack.GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, Backpack.GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, Backpack.GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, Backpack.Scale);
		shader.SetMatrix4("u_Model", model);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(model)));
		Backpack.Render();

		model = glm::translate(glm::mat4(1.0f), Sponza.GetPosition());
		model = glm::rotate(model, Sponza.GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, Sponza.GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, Sponza.GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, Sponza.Scale);
		shader.SetMatrix4("u_Model", model);
		shader.SetMatrix3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
		Sponza.Render();

		model = glm::translate(glm::mat4(1.0f), Planet.Position);
		model = glm::rotate(model, Planet.Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, Planet.Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, Planet.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -(float)glfwGetTime() * 0.05f, glm::vec3(0, 1, 0));
		model = glm::scale(model, Planet.Scale);
		shader.SetMatrix4("u_Model", model);
		shader.SetMatrix3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
		Planet.Render();

		model = glm::translate(glm::mat4(1.0f), Rock.Position);
		model = glm::rotate(model, Rock.Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, Rock.Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, Rock.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, Rock.Scale);
		shader.SetMatrix4("u_Model", model);
		shader.SetMatrix3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
		Rock.Render();




		Rock.Position.x = glm::sin(float(glfwGetTime()) * 0.55f) * 10;
		Rock.Position.z = glm::cos(float(glfwGetTime()) * 0.55f) * 10;

		Rock.Rotation.x += 0.13 * deltaTime;
		Rock.Rotation.z += 0.27 * deltaTime;
		Rock.Rotation.y += 0.02 * deltaTime;

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
			ImGui::DragFloat3("Position", &Planet.GetPosition().x, 0.01f);
			ImGui::DragFloat3("Rotation", &Planet.GetRotation().x, 0.01f);
			ImGui::DragFloat3("Scale", &Planet.GetScale().x, 0.01f);

			ImGui::Separator();

			ImGui::DragFloat3("#Position", &Rock.GetPosition().x, 0.01f);
			ImGui::DragFloat3("#Rotation", &Rock.GetRotation().x, 0.01f);
			ImGui::DragFloat3("#Scale", &Rock.GetScale().x, 0.01f);

			ImGui::Separator();

			ImGui::DragFloat3("##Position", &Backpack.GetPosition().x, 0.01f);
			ImGui::DragFloat3("##Rotation", &Backpack.GetRotation().x, 0.01f);
			ImGui::DragFloat3("##Scale", &Backpack.GetScale().x, 0.01f);

			ImGui::Separator();

			ImGui::DragFloat3("###Position", &Sponza.GetPosition().x, 0.01f);
			ImGui::DragFloat3("###Rotation", &Sponza.GetRotation().x, 0.01f);
			ImGui::DragFloat3("###Scale", &Sponza.GetScale().x, 0.01f);

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
