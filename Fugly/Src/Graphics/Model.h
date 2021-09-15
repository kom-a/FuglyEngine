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

		inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
		size_t LoadMaterialTexutre(const aiMaterial* material, aiTextureType textureType);

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture*> m_Textures;
		std::string m_Directory;

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;

		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_IBO;
	};
}