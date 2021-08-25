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

	private:
		void InitMeshes(aiNode* node, const aiScene* scene);

	private:
		std::vector<Mesh*> m_Meshes;
	};
}