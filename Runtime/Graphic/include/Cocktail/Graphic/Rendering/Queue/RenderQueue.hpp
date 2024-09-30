#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP

#include <queue>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Math/Matrix/Matrix4.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>
#include <Cocktail/Graphic/Material/Material.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp>
#include <Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp>

#include <Cocktail/Renderer/Texture/TextureView.hpp>

namespace Ck
{
	struct StaticMeshRecordInfo
	{
		/**
		 * \brief
		 */
		struct VertexBuffer
		{
			/**
			 * \brief
			 */
			const Renderer::Buffer* Buffer = nullptr;

			/**
			 * \brief
			 */
			std::size_t Offset = 0;

			/**
			 * \brief
			 */
			const VertexLayout* VertexLayout = nullptr;
		};

		/**
		 * \brief The number of vertex (or indices) to be drawn
		 */
		unsigned int Count = 0;

		/**
		 * \brief
		 */
		unsigned int VertexBufferCount = 0;

		/**
		 * \brief
		 */
		VertexBuffer VertexBuffers[Renderer::MaxVertexInputBindingCount];

		/**
		 * \brief The buffer to bind as index buffer
		 */
		const Renderer::Buffer* IndexBuffer = nullptr;

		/**
		 * \brief 
		 */
		Renderer::IndexType IndexType = Renderer::IndexType::Integer;

		/**
		 * \brief
		 */
		std::size_t IndexBufferOffset = 0;

		/**
		 * \brief 
		 */
		Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;

		/**
		 * \brief 
		 */
		bool DoubleSided = false;

		/**
		 * \brief 
		 */
		Matrix4<float> Model;

		/**
		 * \brief 
		 */
		Material::UniformColors UniformColors;

		/**
		 * \brief 
		 */
		EnumMap<Material::TextureType, Ref<Renderer::TextureView>> MaterialTextures;
	};

	/**
	 * \brief 
	 */
	struct CustomRecordInfo
	{
		/**
		 * \brief 
		 */
		using RenderFunction = void(*)(Renderer::CommandList& commandList, void* userData, RecordDrawContext& drawContext);

		/**
		 * \brief 
		 */
		RenderFunction Draw = nullptr;

		/**
		 * \brief 
		 */
		void* UserData = nullptr;
	};

	class COCKTAIL_GRAPHIC_API RenderQueue : public Extends<RenderQueue, Object>
	{
	public:

		/**
		 * \brief 
		 * \param materialProgramManager 
		 * \param shadingMode 
		 */
		RenderQueue(Ref<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode);

		/**
		 * \brief 
		 * \param recordInfo 
		 */
		void PushCustom(const CustomRecordInfo& recordInfo);

		/**
		 * \brief 
		 * \param recordInfo 
		 */
		void PushStaticMesh(const StaticMeshRecordInfo& recordInfo);

		/**
		 * \brief 
		 * \param commandList 
		 * \param drawContext 
		 */
		void Flush(Renderer::CommandList& commandList, RecordDrawContext& drawContext);

		/**
		 * \brief 
		 */
		void Clear();

		/**
		 * \brief 
		 * \return 
		 */
		Material::ShadingMode GetShadingMode() const;

	private:

		Material::ShadingMode mShadingMode;
		EnumMap<RenderableType, Ref<MaterialProgramSet>> mMaterialProgramSets;
		std::queue<Ref<RenderRecord>> mRecordQueue;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
