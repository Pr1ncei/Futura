#include "pch.h"
#include "r_Model.h"

namespace FuturaLibrary
{
	namespace
	{
		void Encapsulate(AxisAlignedBounds& target, const AxisAlignedBounds& source)
		{
			if (!source.IsValid)
				return;

			if (!target.IsValid)
			{
				target = source;
				return;
			}

			target.Min.x = std::min(target.Min.x, source.Min.x);
			target.Min.y = std::min(target.Min.y, source.Min.y);
			target.Min.z = std::min(target.Min.z, source.Min.z);
			target.Max.x = std::max(target.Max.x, source.Max.x);
			target.Max.y = std::max(target.Max.y, source.Max.y);
			target.Max.z = std::max(target.Max.z, source.Max.z);
		}
	}

	Model::Model(const std::string& sourcePath)
		: m_SourcePath(sourcePath)
	{
	}

	void Model::AddSubmesh(const ModelSubmesh& submesh)
	{
		FT_CORE_ASSERT(submesh.MeshAsset, "Model submesh requires a mesh!");

		ModelSubmesh storedSubmesh = submesh;
		if (!storedSubmesh.LocalBounds.IsValid)
			storedSubmesh.LocalBounds = submesh.MeshAsset->GetLocalBounds();

		Encapsulate(m_LocalBounds, storedSubmesh.LocalBounds);
		m_Submeshes.push_back(storedSubmesh);
	}
}
