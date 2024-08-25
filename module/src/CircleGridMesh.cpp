#include "CircleGridMesh.h"

#include <renderservice.h>
#include <nap/core.h>
#include <renderglobals.h>
#include <meshutils.h>
#include <mathutils.h>
#include <cmath>


RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::CircleGridMesh)
	RTTI_CONSTRUCTOR(nap::Core&)
	RTTI_PROPERTY("Sectors", &nap::CircleGridMesh::mSectors, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("Circles", &nap::CircleGridMesh::mCircles, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("Meridians", &nap::CircleGridMesh::mMeridians, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS


namespace nap
{
	CircleGridMesh::CircleGridMesh(Core& core) : mRenderService(core.getService<nap::RenderService>())
	{ }

	bool CircleGridMesh::init(utility::ErrorState& errorState)
	{
		
		std::vector<glm::vec3> positions;
		
		for(int j = 0; j < mCircles; j++)
			for(int i = 0; i < mSectors; i++)
				positions.emplace_back( 0.5f * ((j + 1) / (float)mCircles) * glm::vec3(cos(math::PIX2 * (i / (float)mSectors)), 0.0, sin(math::PIX2 * (i / (float)mSectors))));
		
		// Calculate meridian positions.
		for(int i = 0; i < mMeridians; i++)
			positions.emplace_back( 0.5f * glm::vec3(cos(math::PIX2 * (i / (float)mMeridians)), 0.0, sin(math::PIX2 * (i / (float)mMeridians))));
		
		// Emplace the center position.
		positions.emplace_back(glm::vec3(0,0,0));
		
		
		// Create mesh.
		assert(mRenderService != nullptr);
		mMeshInstance = std::make_unique<nap::MeshInstance>(*mRenderService);
		
		// Add vertex position and color attribute.
		nap::VertexAttribute<glm::vec3>& pattr = mMeshInstance->getOrCreateAttribute<glm::vec3>(vertexid::position);
		
		std::vector<glm::vec3> v_pos;
		v_pos.reserve(positions.size());
		for (const auto& it : positions)
			v_pos.emplace_back((it));
		
		// Set data.
		pattr.setData(v_pos);
		
		// Create shape and generate indicies.
		nap::MeshShape& shape = mMeshInstance->createShape();
		shape.reserveIndices(mSectors * 2);
		
		// Add concentric circles to shape.
		for(int j = 0; j < mCircles; j++)
		{
			int offset = j * mSectors;
			for(int i = 0; i < mSectors - 1; i++)
			{
				shape.addIndex(offset + i);
				shape.addIndex(offset + i+1);
			}
			shape.addIndex(offset + mSectors - 1);
			shape.addIndex(offset + 0);
		}
		
		// Add meridians to shape.
		for(int i = 0; i < mMeridians; i++)
		{
			int offset = mCircles * mSectors;
			shape.addIndex(offset + i);
			shape.addIndex(mCircles * mSectors + mMeridians);
		}
		
		// Set other mesh properties.
		mMeshInstance->setCullMode(ECullMode::None);
		mMeshInstance->setNumVertices(mCircles * mSectors + mMeridians + 1);
		mMeshInstance->setDrawMode(EDrawMode::Lines);
		mMeshInstance->setUsage(EMemoryUsage::Static);
		
		// Initialize.
		return mMeshInstance->init(errorState);
		
	}
}
