#ifndef COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXREF_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXREF_HPP

#include <cstring>

#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Graphic/Export.hpp>

#include <Cocktail/Renderer/Buffer/IndexType.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API IndexConstRef
	{
	public:

		/**
		 * \brief
		 * \return
		 */
		const void* Get() const;

	protected:

		friend class IndexArray;

		/**
		 * \brief
		 * \param indexType
		 * \param indices
		 * \param index
		 * \param indexCount
		 */
		IndexConstRef(Renderer::IndexType indexType, const ByteArray* indices, std::size_t index, std::size_t indexCount);

		Renderer::IndexType mIndexType;
		std::size_t mIndex;
		std::size_t mIndexCount;

	private:

		const ByteArray* mIndices;
	};

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API IndexRef : public IndexConstRef
	{
	public:

		/**
		 * \brief
		 * \tparam T
		 * \return
		 */
		template <typename T>
		void Set(T value) const
		{
			DataType indexDataType = Renderer::ToDataType(mIndexType);
			assert(indexDataType == DataType::Of<T>());

			std::memcpy(reinterpret_cast<T*>(mIndices->GetData() + indexDataType.GetSize() * mIndex), &value, sizeof(T));
		}

	private:

		friend class IndexArray;

		/**
		 * \brief
		 * \param indexType
		 * \param storage
		 * \param index
		 * \param indexCount
		 */
		IndexRef(Renderer::IndexType indexType, ByteArray* storage, std::size_t index, std::size_t indexCount);

		ByteArray* mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXREF_HPP
