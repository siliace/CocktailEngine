#ifndef COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXREF_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXREF_HPP

#include <cstring>

#include <Cocktail/Core/Utility/InOptional.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

#include <Cocktail/Graphic/Export.hpp>

#include <Cocktail/Renderer/Buffer/IndexType.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API IndexRef
	{
	public:

		/**
		 * \brief
		 * \tparam T
		 * \return
		 */
		template <typename T>
		T Get() const
		{
			return Get<T>(InOptional).Get();
		}

		/**
		 * \brief
		 * \tparam T
		 * \return
		 */
		template <typename T>
		Optional<T> Get(InOptionalTag) const
		{
			DataType indexDataType = Renderer::ToDataType(mIndexType);
			if (indexDataType != DataType::Of<T>())
				throw std::invalid_argument("Invalid index type");

			return reinterpret_cast<T*>(mStorage + indexDataType.GetSize() * mIndex);
		}

		/**
		 * \brief
		 * \tparam T
		 * \return
		 */
		template <typename T>
		void Set(T value) const
		{
			DataType indexDataType = Renderer::ToDataType(mIndexType);
			if (indexDataType != DataType::Of<T>())
				throw std::invalid_argument("Invalid index type");

			std::memcpy(reinterpret_cast<T*>(mStorage + indexDataType.GetSize() * mIndex), &value, sizeof(T));
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
		IndexRef(Renderer::IndexType indexType, unsigned char* storage, std::size_t index, std::size_t indexCount);

		Renderer::IndexType mIndexType;
		unsigned char* mStorage;
		std::size_t mIndex;
		std::size_t mIndexCount;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_INDEX_INDEXREF_HPP
