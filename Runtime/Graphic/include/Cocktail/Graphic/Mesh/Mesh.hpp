#ifndef COCKTAIL_GRAPHIC_MESH_MESH_HPP
#define COCKTAIL_GRAPHIC_MESH_MESH_HPP

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>

#include <Cocktail/Renderer/Renderer.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API Mesh : public Extends<Mesh, Object>
	{
	public:

		/**
		 * \brief 
		 * \param graphicEngine
		 * \param vertices 
		 * \param indices 
		 * \param primitiveTopology 
		 */
		Mesh(Ref<GraphicEngine> graphicEngine, Ref<VertexArray> vertices, Ref<IndexArray> indices, Renderer::PrimitiveTopology primitiveTopology);

		/**
		 * \brief 
		 * \return 
		 */
		Ref<VertexBuffer> GetVertexBuffer() const;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<IndexBuffer> GetIndexBuffer() const;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::PrimitiveTopology GetPrimitiveTopology() const;

	private:

		Ref<GraphicEngine> mGraphicEngine;
		Ref<VertexBuffer> mVertexBuffer;
		Ref<IndexBuffer> mIndexBuffer;
		Renderer::PrimitiveTopology mPrimitiveTopology;
	};
}

#endif // COCKTAIL_GRAPHIC_MESH_MESH_HPP
