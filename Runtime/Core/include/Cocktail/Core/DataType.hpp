#ifndef COCKTAIL_CORE_DATATYPE_HPP
#define COCKTAIL_CORE_DATATYPE_HPP

#include <type_traits>

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	/**
	 * \brief Structure describing a primitive type
	 */
	class COCKTAIL_CORE_API DataType
	{
	public:

		/**
		 * \brief DataType matching a signed integer of 1 byte
		 */
		static const DataType Int8;

		/**
		 * \brief DataType matching an unsigned integer of 1 byte
		 */
		static const DataType UnsignedInt8;

		/**
		 * \brief DataType matching a signed integer of 2 bytes
		 */
		static const DataType Int16;

		/**
		 * \brief DataType matching an unsigned integer of 2 bytes
		 */
		static const DataType UnsignedInt16;

		/**
		 * \brief DataType matching a signed integer of 4 bytes
		 */
		static const DataType Int32;

		/**
		 * \brief DataType matching an unsigned integer of 4 bytes
		 */
		static const DataType UnsignedInt32;

		/**
		 * \brief DataType matching a signed integer of 8 bytes
		 */
		static const DataType Int64;

		/**
		 * \brief DataType matching an unsigned integer of 8 bytes
		 */
		static const DataType UnsignedInt64;

		/**
		 * \brief DataType matching a floating point type of 2 bytes
		 */
		static const DataType Float16;

		/**
		 * \brief DataType matching a floating point type of 4 bytes
		 */
		static const DataType Float32;

		/**
		 * \brief DataType matching a floating point type of 8 bytes
		 */
		static const DataType Float64;

		/**
		 * \brief Create a DataType form a primitive type
		 * \return The DataType
		 */
		template <typename T>
		static DataType Of()
		{
			return {
				sizeof(T),
				std::is_floating_point_v<T>,
				std::is_unsigned_v<T>
			};
		}

		/**
		 * \brief Default constructor
		 */
		DataType();

		/**
		 * \brief Constructor
		 * Create a new DataType
		 * \param size The size in byte of the primitive type to describe
		 * \param floatingPoint Specify whether the primitive type to describe is floating point
		 * \param unsigned_ Specify whether the primitive type to describe is unsigned
		 */
		DataType(std::size_t size, bool floatingPoint, bool unsigned_);

		/**
		 * \brief Get the size in bytes of the underlying primitive type
		 * \return The size
		 */
		std::size_t GetSize() const;

		/**
		 * \brief Tell whether the underlying primitive type is a floating point type
		 * \return True if floating point, false otherwise
		 */
		bool IsFloatingPoint() const;

		/**
		 * \brief Tell whether the underlying primitive type is an unsigned type
		 * \return True if the underlying primitive type is unsigned, false otherwise
		 */
		bool IsUnsigned() const;

	private:

		std::size_t mSize;
		bool mFloatingPoint;
		bool mUnsigned;
	};

	/**
	 * \brief Compare two DataTypes
	 * \param lhs The left DataType to compare
	 * \param rhs The right DataType to compare
	 * \return True if /p lhs and /p rhs are strictly identical, false otherwise
	 */
	COCKTAIL_CORE_API bool operator==(const DataType& lhs, const DataType& rhs);

	/**
	 * \brief Compare two DataTypes
	 * \param lhs The left DataType to compare
	 * \param rhs The right DataType to compare
	 * \return True if /p lhs and /p rhs are strictly not identical, false otherwise
	 */
	COCKTAIL_CORE_API bool operator!=(const DataType& lhs, const DataType& rhs);
}

#endif // COCKTAIL_CORE_DATATYPE_HPP
