#ifndef COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXARRAY_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXARRAY_HPP

#include <memory>

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Geometry/Index/IndexRef.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class COCKTAIL_GRAPHIC_API IndexArray : public Extends<IndexArray, Object>
	{
	public:

		/**
		 * \brief
		 * \param indexType
		 * \param indexCount
		 */
		IndexArray(Renderer::IndexType indexType, std::size_t indexCount);

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		IndexRef At(std::size_t index) const;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::IndexType GetIndexType() const;

		/**
		 * \brief
		 * \return
		 */
		std::size_t GetIndexCount() const;

		/**
		 * \brief
		 * \return
		 */
		const void* GetPointer() const;

	private:

		Renderer::IndexType mIndexType;
		std::size_t mIndexCount;
		std::unique_ptr<unsigned char[]> mStorage;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXARRAY_HPP
