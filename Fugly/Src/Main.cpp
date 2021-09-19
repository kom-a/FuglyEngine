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
    
	//Renderer renderer;
    
	glClearColor(0.7f, 0.8f, 0.9f, 1.0f);
    
	float lastTime = glfwGetTime();
	float unprocessedTime = 0;
	int fps = 0;
    
	FrameBuffer FBO(window.GetWidth(), window.GetHeight());

	//Model Sponza("Res/Models/sponza/sponza.obj");
	Model Backpack("Res/Models/Backpack.obj");
	Cube cube(glm::vec3(0.1));
	Plane plane;

	// Model Sponza("Res/Models/sponza/sponza.obj");

	Shader shader("Res/Shaders/TestVertex.glsl", "Res/Shaders/TestFragment.glsl");
	Shader singleColorShader("Res/Shaders/TestVertex.glsl", "Res/Shaders/FragmentShader.glsl");
	glm::mat4 testModel(1.0f);
	glm::mat4 testProjection = glm::perspective(glm::radians(45.0f), window.Aspect(), 0.1f, 250.0f);
    
	Camera camera(glm::vec3(0, 2, 10));
	
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	// position - 3, texCoords - 2
	float quadVertices[] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
	};

	unsigned int quadIndices[] = {
		0, 1, 2, 0, 2, 3
	};

	VertexArray VAO;
	VAO.Bind();
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	unsigned IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));

	Texture grassTexture("Res/Window.png", 0);
	Shader quadShader("Res/Shaders/QuadVertex.glsl", "Res/Shaders/QuadFragment.glsl");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	std::vector<std::string> faces
	{	
		"Res/skybox/right.jpg",
		"Res/skybox/left.jpg",
		"Res/skybox/top.jpg",
		"Res/skybox/bottom.jpg",
		"Res/skybox/front.jpg",
		"Res/skybox/back.jpg"
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
		
		FBO.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glDepthMask(GL_FALSE);
		skyboxShader.Bind();
		skyboxShader.SetMatrix4("u_Projection", testProjection);
		skyboxShader.SetMatrix4("u_View", glm::mat4(glm::mat3(camera.GetViewMatrix())));
		skyboxShader.SetUniform1i("skybox", 0);

		cubeMapVAO.Bind();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		
		testModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		shader.Bind();
		shader.SetMatrix4("u_Projection", testProjection);
		shader.SetMatrix4("u_View", camera.GetViewMatrix());
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		shader.SetUniform3f("u_CameraPos", camera.GetPosition());
        
		shader.SetUniform1i("diffuseSampler", 0);
		shader.SetUniform1i("specularSampler", 1);

		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0));
		testModel = glm::scale(testModel, glm::vec3(25, 0.25, 25));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		plane.Render();

		testModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.75, 0));
		testModel = glm::rotate(testModel, float(glfwGetTime()), glm::vec3(0, 1, 0));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		Backpack.Render();

		testModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		shader.SetMatrix4("u_Model", testModel);
		shader.SetMatrix3("u_NormalMatrix", glm::transpose(glm::inverse(testModel)));
		// Sponza.Render();

		FBO.Unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		VAO.Bind();
		quadShader.Bind();
		quadShader.SetUniform1i("sampler", 0);
		glDisable(GL_DEPTH_TEST);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, FBO.GetColorBuffer());
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
		if (Mouse::IsButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) &&
			mouse.x > sceneWindowPos.x && mouse.x < sceneWindowPos.x + sceneWindowSize.x &&
			mouse.y > sceneWindowPos.y && mouse.y < sceneWindowPos.y + sceneWindowSize.y)
		{
			window.SetMouseEnabled(false);
			camera.Update(deltaTime);
		}
		else
		{
			camera.ResetFirstMouse();
			window.SetMouseEnabled(true);
		}
		
		window.Update();
		fps++;
	}
    
	return 0;
}