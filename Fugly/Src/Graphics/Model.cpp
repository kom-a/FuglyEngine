#include "Model.h"

#include "../Utils/Log.h"


namespace Fugly
{
	Model::Model(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, 
			aiProcess_Triangulate | aiProcess_GenNormals);
		if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
		{
			LOG_ERROR("Failed to load model \"{0}\"", filename);
			return;
		}

		ProcessNode(scene->mRootNode, scene);
	}

	Model::~Model()
	{
	}

	void Model::Render()
	{
		for (Mesh& m : m_Meshes)
			m.Render();
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene, node->mTransformation));
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			aiVector3D position = transformation * mesh->mVertices[i];
			aiVector3D normal = (transformation * mesh->mNormals[i]).Normalize();
			
			vertex.position.x = position.x;
			vertex.position.y = position.y;
			vertex.position.z = position.z;

			vertex.normal.x = normal.x;
			vertex.normal.y = normal.y;
			vertex.normal.z = normal.z;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i]; 
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return Mesh(vertices, indices);
	}
}


