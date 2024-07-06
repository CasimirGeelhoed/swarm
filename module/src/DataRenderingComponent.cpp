#include "DataRenderingComponent.h"

// External Includes
#include <entity.h>
#include <renderservice.h>
#include <nap/core.h>
#include <glm/gtc/matrix_transform.hpp>
#include <mathutils.h>
#include <renderglobals.h>

RTTI_BEGIN_CLASS(nap::DataRenderingComponent)
	RTTI_PROPERTY("MaterialInstance",    &nap::DataRenderingComponent::mMaterialInstanceResource,    nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("Mesh",            &nap::DataRenderingComponent::mMesh,                    nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("Data",            &nap::DataRenderingComponent::mData,                    nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("DynamicColoring",  &nap::DataRenderingComponent::mDynamicColoring, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::DataRenderingComponentInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS


namespace nap
{
	void DataRenderingComponent::getDependentComponents(std::vector<rtti::TypeInfo>& components) const
	{
		components.emplace_back(RTTI_OF(nap::TransformComponent));
	}


	DataRenderingComponentInstance::DataRenderingComponentInstance(EntityInstance& entity, Component& resource) :
	RenderableComponentInstance(entity, resource),
	mRenderService(entity.getCore()->getService<RenderService>())    { }

	bool DataRenderingComponentInstance::init(utility::ErrorState& errorState)
	{
		if (!RenderableComponentInstance::init(errorState))
			return false;
		
		// Get resource
		DataRenderingComponent* resource = getComponent<DataRenderingComponent>();
		
		// Get data pointer
		mData = resource->mData.get();
		
		// Fetch transform, used to offset the copied meshes
		mTransform = getEntityInstance()->findComponent<TransformComponentInstance>();
		if (!errorState.check(mTransform != nullptr,
							  "%s: unable to find transform component", resource->mID.c_str()))
			return false;
		
		// Initialize our material instance based on values in the resource
		if (!mMaterialInstance.init(*mRenderService, resource->mMaterialInstanceResource, errorState))
			return false;
		
		// Get handle to matrices, which we set in the draw call
		UniformStructInstance* mvp_struct = mMaterialInstance.getOrCreateUniform(uniform::mvpStruct);
		if (mvp_struct != nullptr)
		{
			mModelMatUniform = mvp_struct->getOrCreateUniform<UniformMat4Instance>(uniform::modelMatrix);
			mViewMatUniform = mvp_struct->getOrCreateUniform<UniformMat4Instance>(uniform::viewMatrix);
			mProjectMatUniform = mvp_struct->getOrCreateUniform<UniformMat4Instance>(uniform::projectionMatrix);
		}
		
		// Create renderable mesh from draw mesh
		mMesh = mRenderService->createRenderableMesh(*resource->mMesh, mMaterialInstance, errorState);
		if (!errorState.check(mMesh.isValid(), "Mesh can't be copied"))
			return false;
		
		// Get the flag that determines whether colors are being set by the 'displayIntensity' field.
		mDynamicColoring = resource->mDynamicColoring;
		
		if(mDynamicColoring)
		{
			// Get handle to color uniform, which we set in the draw call. Save the initial color as default color.
			mColorUniform = mMaterialInstance.getOrCreateUniform("UBO")->getOrCreateUniform<UniformVec3Instance>("color");
			if (!errorState.check(mColorUniform, "Color uniform not found."))
				return false;
			mBaseColor = mColorUniform->getValue();
		}
		
		return true;
	}


	void DataRenderingComponentInstance::update(double deltaTime)
	{
	}


	static void renderMesh(RenderService& renderService, RenderService::Pipeline& pipeline, RenderableMesh& renderableMesh, VkCommandBuffer commandBuffer)
	{
		// Get material to render with and descriptors for material
		MaterialInstance& mat_instance = renderableMesh.getMaterialInstance();
		const DescriptorSet& descriptor_set = mat_instance.update();
		
		// Bind descriptor set for next draw call
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.mLayout, 0, 1, &descriptor_set.mSet, 0, nullptr);
		
		// Bind vertex buffers
		const std::vector<VkBuffer>& vertexBuffers = renderableMesh.getVertexBuffers();
		const std::vector<VkDeviceSize>& vertexBufferOffsets = renderableMesh.getVertexBufferOffsets();
		vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), vertexBufferOffsets.data());
		
		// Get mesh to draw
		MeshInstance& mesh_instance = renderableMesh.getMesh().getMeshInstance();
		GPUMesh& mesh = mesh_instance.getGPUMesh();
		
		// Draw individual shapes inside mesh
		for (int index = 0; index < mesh_instance.getNumShapes(); ++index)
		{
			const IndexBuffer& index_buffer = mesh.getIndexBuffer(index);
			vkCmdBindIndexBuffer(commandBuffer, index_buffer.getBuffer(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandBuffer, index_buffer.getCount(), 1, 0, 0, 0);
		}
	}


	void DataRenderingComponentInstance::onDraw(IRenderTarget& renderTarget, VkCommandBuffer commandBuffer, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		RenderService* renderService = getEntityInstance()->getCore()->getService<RenderService>();
		
		// Set mvp matrices in material (if present)
		if (mProjectMatUniform != nullptr)
			mProjectMatUniform->setValue(projectionMatrix);
		if (mViewMatUniform != nullptr)
			mViewMatUniform->setValue(viewMatrix);
		
		// Get points to copy onto
		const std::vector<glm::vec3>& pos_data = mData->getVec3Field("displayPosition");
		const std::vector<float>& scale_data = mData->getFloatField("displayScale");
		const std::vector<float>& intensity_data = mData->getFloatField("displayIntensity");
		bool hasScaleData = scale_data.size() == pos_data.size();
		bool hasIntensityData = intensity_data.size() == pos_data.size();
		
		// Get render-pipeline for mesh / material
		utility::ErrorState error_state;
		RenderService::Pipeline pipeline = mRenderService->getOrCreatePipeline(renderTarget, mMesh.getMesh(), mMesh.getMaterialInstance(), error_state);
		
		// Bind pipeline per mesh we are going to render
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.mPipeline);
		
		// Update scissor state
		VkRect2D scissor_rect
		{
			{ 0, 0 },
			{
				(uint32_t)renderTarget.getBufferSize().x,
				(uint32_t)renderTarget.getBufferSize().y
			}
		};
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor_rect);
		
		// Render at all data positions
		for (auto i = 0; i < pos_data.size(); i++)
		{
			// Calculate model matrix
			glm::mat4x4 mat = glm::translate(mTransform->getGlobalTransform(), pos_data[i]);
			if(hasScaleData)
				mat = glm::scale(mat, glm::vec3(scale_data[i]));
			else
				mat = glm::scale(mat, mDefaultScale);
			mModelMatUniform->setValue(mat);
			
			// Set color
			if(mDynamicColoring && hasIntensityData)
			{
				glm::vec3 color = intensity_data[i] * mBaseColor + (1.f - intensity_data[i]) * glm::vec3(0.5, 0.5, 0.5);
				mColorUniform->setValue(color);
			}
			
			// Render mesh
			renderMesh(*mRenderService, pipeline, mMesh, commandBuffer);
		}
	}
}
