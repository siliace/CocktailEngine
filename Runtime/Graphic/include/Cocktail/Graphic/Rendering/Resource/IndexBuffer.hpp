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
		IndexBuffer(std::shared_ptr<GraphicEngine> graphicEngine, std::shared_ptr<IndexArray> indices, std::string_view name = "");

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
		std::shared_ptr<IndexArray> GetIndexArray() const;

    private:

        std::shared_ptr<IndexArray> mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP
