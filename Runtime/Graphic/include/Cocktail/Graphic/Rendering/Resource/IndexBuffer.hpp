#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>
#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API IndexBuffer : public BufferResource
	{
	public:

		/**
		 * \brief 
		 * \param graphicEngine 
		 * \param indices
		 * \param name 
		 */
		IndexBuffer(SharedPtr<GraphicEngine> graphicEngine, SharedPtr<IndexArray> indices, const AnsiChar* name = "");

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
		SharedPtr<IndexArray> GetIndexArray() const;

    private:

        SharedPtr<IndexArray> mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP
