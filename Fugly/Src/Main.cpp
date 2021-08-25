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

	glClearColor(0.125f, 0.125f, 0.125f, 1.0f);

	float lastTime = glfwGetTime();
	float unprocessedTime = 0;
	int fps = 0;

	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), window.Aspect(), 0.1f, 100.0f);

	float speed = 2.0f;

	Sprite sprite(glm::vec3(0, 0, -50.0f), glm::vec3(50, 50, 0.0f), glm::vec4(0.8f, 0.5f, 0.3f, 1.0f));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("Res/Backpack.fbx", aiProcess_Triangulate | aiProcess_FlipUVs);
	aiNode* rootNode = scene->mRootNode;

	if (!scene || !rootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		LOG_ERROR("Failed to load model");
	}

	size_t indexOffset = 0;

	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D& pos = mesh->mVertices[i];
			Positions.push_back(pos.x);
			Positions.push_back(pos.y);
			Positions.push_back(pos.z);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			for (size_t i = 0; i < face.mNumIndices; i++)
			{
				Indices.push_back(face.mIndices[i] + indexOffset);
			}
		}

		indexOffset += mesh->mNumVertices;

	}


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
	glm::mat4 testProjection = glm::perspective(glm::radians(45.0f), window.Aspect(), 0.01f, 100.0f);

	Camera camera(glm::vec3(0, 0, 8), glm::vec3(0, 0, -1));
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

		camera.Update(deltaTime);
		window.Update();
		fps++;
	}

	return 0;
}