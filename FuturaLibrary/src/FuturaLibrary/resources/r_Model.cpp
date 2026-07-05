#include "pch.h"
#include "r_Model.h"

namespace FuturaLibrary
{
	Model::Model(const std::string& sourcePath)
		: m_SourcePath(sourcePath)
	{
	}

	void Model::AddSubmesh(const ModelSubmesh& submesh)
	{
		FT_CORE_ASSERT(submesh.MeshAsset, "Model submesh requires a mesh!");
		m_Submeshes.push_back(submesh);
	}
}
