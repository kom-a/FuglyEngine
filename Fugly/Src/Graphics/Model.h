#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

namespace Fugly
{
	class Model
	{
	public:
		Model(const std::string& filename);
		~Model();

		void Render();

		inline const std::vector<Mesh*>& GetMeshes() const { return m_Meshes; }
		inline glm::vec3& GetPosition() { return Position; }
		inline glm::vec3& GetRotation() { return Rotation; }
		inline glm::vec3& GetScale() { return Scale; }

	public:
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
		size_t LoadMaterialTexutre(const aiMaterial* material, aiTextureType textureType);

	private:
		std::vector<Mesh*> m_Meshes;
		std::vector<Texture*> m_Textures;
		std::string m_Directory;

		size_t meshBytes = 0;
		size_t texBytes = 0;
	};
}
