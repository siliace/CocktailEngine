#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORDINFO_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORDINFO_HPP

#include <Cocktail/Core/Math/Matrix/Matrix4.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Renderer.hpp>
#include <Cocktail/Renderer/Buffer/Buffer.hpp>
#include <Cocktail/Renderer/Texture/TextureView.hpp>

#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>
#include <Cocktail/Graphic/Material/Material.hpp>

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
		unsigned int FirstVertex = 0;

		/**
		 * \brief 
		 */
		unsigned int FirstIndex = 0;

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

		LinearColor MaterialBaseColor;
		LinearColor MaterialSpecularColor;
		LinearColor MaterialEmissiveColor;

		/**
		 * \brief Sampler to use to access to material's textures
		 */
		std::shared_ptr<Renderer::Sampler> Sampler;

		/**
		 * \brief 
		 */
		EnumMap<Material::TextureType, std::shared_ptr<Renderer::TextureView>> MaterialTextures;

		/**
		 * \brief Tell whether the geometry drawn by this record is opaque.
		 * If record is not opaque, it will be drawn using blending to perform transparency.
		 */
		bool Opaque;
	};    
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORDINFO_HPP
