#include "Model.h"

#include <GL/glew.h>

#include "../Utils/Log.h"


namespace Fugly
{
	Model::Model(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, 
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
		if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
		{
			LOG_ERROR("Failed to load model \"{0}\"", filename);
			return;
		}

		// Save model's directory to load textures later
		m_Directory = filename.substr(0, filename.find_last_of('/') + 1); // +1 to include slash in "m_Directory"

		ProcessNode(scene->mRootNode, scene);
	}


	Model::~Model()
	{
		for (int i = 0; i < m_Textures.size(); i++)
		{
			delete m_Textures[i];
		}
	}

	void Model::Render()
	{
		for (Mesh& mesh : m_Meshes)
		{
			m_Textures[mesh.GetDiffuseTextureIndex()]->Bind(0);
			mesh.Render();
		}
			
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

			aiVector3D position = mesh->mVertices[i];
			aiVector3D normal = (mesh->mNormals[i]).Normalize();
			
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

		size_t textureIndex = 0;
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			textureIndex = LoadMaterialTexutres(material, aiTextureType_DIFFUSE);
		}

		return Mesh(vertices, indices, textureIndex);
	}

	size_t Model::LoadMaterialTexutres(const aiMaterial* material, aiTextureType textureType)
	{
		for (size_t i = 0; i < material->GetTextureCount(textureType); i++)
		{
			aiString str;			
			material->GetTexture(textureType, i, &str);
			std::string textureFilename = m_Directory + str.C_Str();

			bool skipLoading = false;
			for(size_t j = 0; j < m_Textures.size(); j++)
			{
				if (m_Textures[j]->GetPath() == textureFilename)
				{
					m_Textures.push_back(m_Textures[j]);
					skipLoading = true;
					break;
				}
			}

			if (skipLoading) continue;

			m_Textures.push_back(new Texture(textureFilename, 0));
			LOG_DEBUG("{0} texture loaded \"{1}\"", textureType, textureFilename);
		}

		return m_Textures.size() - 1;
	}
}


