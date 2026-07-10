/**
 *  @file r_Model.h
 *
 *  @brief Declares imported model resources composed of engine meshes and materials.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 05, 2026
 *      Last Modified on:    July 05, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/renderer/r_Material.h"
#include "FuturaLibrary/renderer/r_Mesh.h"

namespace FuturaLibrary
{
	struct ModelSubmesh
	{
		std::string Name;
		Ref<Mesh> MeshAsset;
		Ref<Material> MaterialAsset;
		AxisAlignedBounds LocalBounds;
		uint32_t MaterialIndex = 0;
	};

	class FT_API Model
	{
	public:
		Model() = default;
		explicit Model(const std::string& sourcePath);

		void AddSubmesh(const ModelSubmesh& submesh);

		const std::string& GetSourcePath() const { return m_SourcePath; }
		const std::vector<ModelSubmesh>& GetSubmeshes() const { return m_Submeshes; }
		const AxisAlignedBounds& GetLocalBounds() const { return m_LocalBounds; }
		bool IsEmpty() const { return m_Submeshes.empty(); }

	private:
		std::string m_SourcePath;
		std::vector<ModelSubmesh> m_Submeshes;
		AxisAlignedBounds m_LocalBounds;
	};
}
