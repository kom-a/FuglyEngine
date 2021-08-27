#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

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
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	private:
		std::vector<Mesh> m_Meshes;
	};
}