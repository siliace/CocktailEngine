#ifndef COCKTAIL_CORE_MEMORY_MEMORY_HPP
#define COCKTAIL_CORE_MEMORY_MEMORY_HPP

#include <cstring>
#include <type_traits>

namespace Ck
{
	/**
	 * \class AllocatorUtils
	 * \brief Utility class providing low-level memory operations for allocators.
	 *
	 * This class contains static functions for constructing, moving,
	 * and destroying objects in raw memory. It is designed to be used
	 * by custom allocators and low-level memory management routines.
	 */
	class AllocatorUtils
	{
	public:

		/**
		 * \brief Constructs an object in-place at the given memory location
		 * \tparam T Type of the object to construct
		 * \tparam TArgs Variadic template arguments to forward to T's constructor
		 * \param element Pointer to uninitialized memory where the object will be constructed
		 * \param args Arguments to forward to T's constructor
		 */
		template <typename T, typename... TArgs>
		static void Construct(T* element, TArgs&&... args)
		{
			new (element) T(std::forward<TArgs>(args)...);
		}

		/**
		 * \brief Constructs multiple objects in-place in a contiguous memory block
		 * \tparam T Type of the objects to construct
		 * \tparam TSizeType Integral type used for the size/count
		 * \tparam TArgs Variadic template arguments to forward to T's constructor
		 * \param size Number of objects to construct
		 * \param elements Pointer to uninitialized memory block
		 * \param args Arguments to forward to each object's constructor
		 * \note Memory must be properly aligned for type T
		 */
		template <typename T, typename TSizeType, typename... TArgs>
		static void ConstructRange(TSizeType size, T* elements, TArgs&&... args)
		{
			static_assert(std::is_integral_v<TSizeType>, "TSizeType must be an integral type");

			assert(elements != nullptr);

			for (TSizeType i = 0; i < size; ++i)
				new (&elements[i]) T(std::forward<TArgs>(args)...);
		}

		/**
		 * \brief Copies a range of object from source memory to destination memory
		 * \tparam TDestination Type of objects to copy to
		 * \tparam TSource Type of objects to copy from
		 * \tparam TSizeType Integral type used for the size/count
		 * \param size Number of objects to copy
		 * \param destination Pointer to the destination memory block
		 * \param destination Pointer to the source memory block
		 * \note Source and destination should not overlap. Memory must be aligned properly.
		 */
		template <typename TDestination, typename TSource, typename TSizeType,
			typename = std::enable_if_t<std::is_constructible_v<TDestination, TSource>>
		>
		static void CopyRange(TSizeType size, TDestination* destination, const TSource* source)
		{
			static_assert(std::is_integral_v<TSizeType>, "TSizeType must be an integral type");

			assert(source != nullptr && destination != nullptr);

			for (TSizeType i = 0; i < size; ++i)
				new (&destination[i]) TDestination(source[i]);
		}

		/**
		 * \brief Moves a range of objects from source to destination memory
		 * \tparam T Type of objects to move (must be nothrow move constructible)
		 * \tparam TSizeType Integral type used for the size/count
		 * \param size Number of objects to move
		 * \param destination Pointer to the destination memory block
		 * \param source Pointer to the source memory block
		 * \note Source and destination may overlap. Non-trivial types are moved individually,
		 *       trivially copyable types are memmoved. Memory must be aligned properly
		 */
		template <typename T, typename TSizeType>
		static void MoveRange(TSizeType size, T* destination, T* source)
		{
			static_assert(std::is_nothrow_move_constructible_v<T>, "T must be nothrow move constructible");
			static_assert(std::is_nothrow_destructible_v<T>, "T must be nothrow destructible");

			assert(source != nullptr && destination != nullptr);

			if constexpr (!std::is_trivially_copyable_v<T>)
			{
				if (destination < source)
				{
					for (TSizeType i = 0; i < size; ++i)
					{
						T& element = source[i];
						new (&destination[i]) T(std::move(element));
						element.~T();
					}
				}
				else
				{
					for (TSizeType i = size; i-- > 0; )
					{
						T& element = source[i];
						new (&destination[i]) T(std::move(element));
						element.~T();
					}
				}
			}
			else
			{
				std::memmove(destination, source, sizeof(T) * size);
			}
		}

		/**
		 * \brief Destroys a single object
		 * \tparam T Type of the object
		 * \param element Pointer to the object to destroy
		 * \note Only calls the destructor if T is non-trivially destructible
		 */
		template <typename T>
		static void Destroy(T* element)
		{
			assert(element != nullptr);

			if constexpr (!std::is_trivially_destructible_v<T>)
				element->~T();
		}

		/**
		 * \brief Destroys a single object
		 * \tparam T Type of the object
		 * \param element Reference to the object to destroy
		 * \note Only calls the destructor if T is non-trivially destructible
		 */
		template <typename T>
		static void Destroy(T& element)
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
				element.~T();
		}

		/**
		 * \brief Destroys a contiguous range of objects
		 * \tparam T Type of objects
		 * \tparam TSizeType Integral type used for the size/count
		 * \param size Number of objects to destroy
		 * \param elements Pointer to the first object in the range
		 * \note Destruction is performed in reverse order. Only non-trivially destructible
		 *       types have their destructor called. Memory must be aligned properly
		 */
		template <typename T, typename TSizeType>
		static void DestroyRange(TSizeType size, T* elements)
		{
			static_assert(std::is_integral_v<TSizeType>, "TSizeType must be an integral type");

			assert(elements != nullptr);

			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				for (TSizeType i = size; i-- > 0; )
					elements[i].~T();
			}
		}
	};
}

#endif // COCKTAIL_CORE_MEMORY_MEMORY_HPP
