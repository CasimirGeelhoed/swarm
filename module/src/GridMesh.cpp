/* Copyright (C) 2024 Casimir Geelhoed */

#include "GridMesh.h"

#include <renderservice.h>
#include <nap/core.h>
#include <renderglobals.h>
#include <meshutils.h>
#include <mathutils.h>
#include <cmath>


RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::GridMesh)
	RTTI_CONSTRUCTOR(nap::Core&)
	RTTI_PROPERTY("Lines", &nap::GridMesh::mLines, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS


namespace nap
{
	GridMesh::GridMesh(Core& core) : mRenderService(core.getService<nap::RenderService>())
	{ }

	bool GridMesh::init(utility::ErrorState& errorState)
	{
		
		std::vector<glm::vec3> positions;
		
		for(int i = 0; i < mLines; i++)
		{
			positions.emplace_back(glm::vec3(i / (mLines - 1.0f) - 0.5f, 0, -0.5));
			positions.emplace_back(glm::vec3(i / (mLines - 1.0f) - 0.5f, 0, 0.5));
			positions.emplace_back(glm::vec3(-0.5, 0, i / (mLines - 1.0f) - 0.5f));
			positions.emplace_back(glm::vec3(0.5, 0, i / (mLines - 1.0f) - 0.5f));
		}
		
		// Create mesh.
		assert(mRenderService != nullptr);
		mMeshInstance = std::make_unique<nap::MeshInstance>(*mRenderService);
		
		// Add vertex position.
		nap::VertexAttribute<glm::vec3>& pattr = mMeshInstance->getOrCreateAttribute<glm::vec3>(vertexid::position);
		
		std::vector<glm::vec3> v_pos;
		v_pos.reserve(positions.size());
		for (const auto& it : positions)
			v_pos.emplace_back((it));
		
		// Set data.
		pattr.setData(v_pos);
		
		// Create shape and generate indicies.
		nap::MeshShape& shape = mMeshInstance->createShape();
		shape.reserveIndices(mLines * 4);
		
		// Create grid of lines.
		for(int i = 0; i < mLines; i++)
		{
			shape.addIndex(i * 4);
			shape.addIndex(i * 4 + 1);
			shape.addIndex(i * 4 + 2);
			shape.addIndex(i * 4 + 3);
		}
		
		// Set other mesh properties.
		mMeshInstance->setCullMode(ECullMode::None);
		mMeshInstance->setNumVertices(mLines * 4);
		mMeshInstance->setDrawMode(EDrawMode::Lines);
		mMeshInstance->setUsage(EMemoryUsage::Static);
		
		// Initialize.
		return mMeshInstance->init(errorState);
	}
}
