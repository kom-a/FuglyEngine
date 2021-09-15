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

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, texCoords)));
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
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
#if 1
		for (Mesh& mesh : m_Meshes)
		{
			m_Textures[mesh.GetDiffuseTextureIndex()]->Bind(0);
			m_Textures[mesh.GetSpecularTextureIndex()]->Bind(1);
			mesh.Render();
		}
#else
		m_Textures[0]->Bind(0);
		m_Textures[0]->Bind(1);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
#endif	
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

		size_t diffuseTextureIndex = 0;
		size_t specularTextureIndex = 0;
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			
			diffuseTextureIndex = LoadMaterialTexutre(material, aiTextureType_DIFFUSE);
			specularTextureIndex = LoadMaterialTexutre(material, aiTextureType_SPECULAR);
		}

		// TODO: Complete batching
		
		for (size_t i = 0; i < indices.size(); i++)
		{
			m_Indices.push_back(indices[i] + m_Vertices.size());
		}

		m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());

		return Mesh(vertices, indices, diffuseTextureIndex, specularTextureIndex);
	}

	size_t Model::LoadMaterialTexutre(const aiMaterial* material, aiTextureType textureType)
	{
		aiString str;
		material->GetTexture(textureType, 0, &str);
		std::string textureFilename = m_Directory + str.C_Str();

		for(size_t j = 0; j < m_Textures.size(); j++)
		{
			if (m_Textures[j]->GetPath() == textureFilename)
				return j;
		}

		m_Textures.push_back(new Texture(textureFilename, 0));

		return m_Textures.size() - 1;
	}
}
