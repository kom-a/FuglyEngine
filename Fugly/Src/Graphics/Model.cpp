#include "Model.h"

#include "../Utils/Log.h"


namespace Fugly
{
	Model::Model(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
		{
			LOG_ERROR("Failed to load model \"{0}\"", filename);
			return;
		}

		InitMeshes(scene->mRootNode, scene);
	}

	Model::~Model()
	{
		for (int i = 0; i < m_Meshes.size(); i++)
			delete m_Meshes[i];
	}

	void Model::InitMeshes(aiNode* node, const aiScene* scene)
	{
		
	}
}


