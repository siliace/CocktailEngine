#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>
#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API IndexBuffer : public Extends<IndexBuffer, BufferResource>
	{
	public:

		/**
		 * \brief 
		 * \param graphicEngine 
		 * \param indices
		 * \param name 
		 */
		IndexBuffer(Ref<GraphicEngine> graphicEngine, Ref<IndexArray> indices, std::string_view name = "");

		/**
		 * \brief
		 */
		void Upload();

		/**
		 * \brief 
		 */
		void Upload(std::size_t firstIndex, std::size_t indexCount);

		/**
		 * \brief 
		 * \return 
		 */
		Ref<IndexArray> GetIndexArray() const;

    private:

        Ref<IndexArray> mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP
