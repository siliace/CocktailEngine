#ifndef COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXARRAY_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXARRAY_HPP

#include <memory>

#include <Cocktail/Core/Meta/Extends.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

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
		 * \brief Merge the content of an IndexArray inside this one
		 * \param other The IndexArray to merge
		 */
		void Merge(const IndexArray& other);

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		IndexRef At(std::size_t index);

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		IndexConstRef At(std::size_t index) const;

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
		const ByteArray& GetIndices() const;

	private:

		Renderer::IndexType mIndexType;
		std::size_t mIndexCount;
		ByteArray mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXARRAY_HPP
