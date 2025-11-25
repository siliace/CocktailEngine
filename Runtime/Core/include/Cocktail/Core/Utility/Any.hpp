#ifndef COCKTAIL_CORE_UTILITY_ANY_HPP
#define COCKTAIL_CORE_UTILITY_ANY_HPP

#include <memory>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
	/**
	 * \brief Exception type for bad Any casts
	 * This exception is thrown when trying to cast an any to a type that's not the underlying one.
	 */
	COCKTAIL_DECLARE_EXCEPTION_FROM(AnyCastException, RuntimeException);

	/**
	 * \brief A type-safe container for single arbitrary values
	 */
	class COCKTAIL_CORE_API Any
	{
	public:

		/**
		 * \brief Constructs an empty Any
		 */
		Any() = default;

		/**
		 * \brief Constructor
		 * Create a new instance of Any by copying a value of type T.
		 *
		 * \tparam T Type of the value to store
		 *
		 * \param value The value to copy into the Any
		 */
		template <typename T>
		explicit Any(const T& value)
		{
			mHolder = MakeUnique<AnyHolder<T>>(value);
		}

		/**
		 * \brief Constructor
		 * Create a new instance of Any by moving a value of type T.
		 *
		 * \tparam T Type of the value to store
		 *
		 * \param value The value to move into the Any
		 */
		template <typename T>
		explicit Any(T&& value)
		{
			mHolder = MakeUnique<AnyHolder<T>>(std::move(value));
		}

		/**
		 * \brief Constructor
		 * Create a new instance of Any by constructing in-place a value using the provided arguments.
		 *
		 * \tparam T Type to construct inside the Any
		 * \tparam Args Argument types used to construct T
		 *
		 * \param args Arguments to forward to T's constructor
		 */
		template <typename T, typename... Args>
		explicit Any(InPlaceTypeTag<T>, Args&&... args)
		{
			mHolder = MakeUnique<AnyHolder<T>>(InPlace, std::forward<Args>(args)...);
		}

		/**
		 * \brief Checks if the Any is empty
		 *
		 * \return True if no value is stored
		 */
		bool IsEmpty() const;

		/**
		 * \brief Resets the Any to an empty state
		 */
		void Reset();

		/**
		 * \brief Swaps the contents with another Any
		 *
		 * \param other The other Any to swap with
		 */
		void Swap(Any& other);

		/**
		 * \brief Retrieves the stored value of type U
		 *
		 * \tparam U Type to retrieve
		 *
		 * \return A reference to the stored value
		 *
		 * \throws AnyCastException If the stored type is not U
		 */
		template <typename U>
		U& Get()
		{
			return TryGet<U>().template GetOrThrow<AnyCastException>();
		}

		/**
		 * \brief Retrieves the stored value of type U
		 *
		 * \tparam U Type to retrieve
		 *
		 * \return A const reference to the stored value
		 *
		 * \throws AnyCastException If the stored type is not U
		 */
		template <typename U>
		const U& Get() const
		{
			return TryGet<U>().template GetOrThrow<AnyCastException>();
		}

		/**
		 * \brief Attempts to retrieve the stored value of type U
		 *
		 * \tparam U Type to retrieve
		 *
		 * \return An Optional containing a reference to the stored value if U type matched, otherwise empty
		 */
		template <typename U>
		Optional<U&> TryGet()
		{
			if (mHolder && mHolder->GetTypeInfo() == typeid(U))
			{
				return Optional<U&>::Of(
					static_cast<AnyHolder<U>*>(mHolder.Get())->Get()
				);
			}

			return Optional<U&>::Empty();
		}

		/**
		 * \brief Attempts to retrieve the stored value of type U
		 *
		 * \tparam U Type to retrieve
		 *
		 * \return An Optional containing a const reference to the stored value if U type matched, otherwise empty
		 */
		template <typename U>
		Optional<const U&> TryGet() const
		{
			if (mHolder && mHolder->GetTypeInfo() == typeid(U))
			{
				return Optional<const U&>::Of(
					static_cast<AnyHolder<U>*>(mHolder.Get())->Get()
				);
			}

			return Optional<const U&>::Empty();
		}

	private:

		/**
		 * \brief Abstract base class used for type erasure
		 */
		class AbstractAnyHolder
		{
		public:

			/**
			 * \brief Default destructor
			 */
			virtual ~AbstractAnyHolder() = default;

			/**
			 * \brief Returns the stored type information
			 *
			 * \return std::type_info of the stored type
			 */
			virtual const std::type_info& GetTypeInfo() const = 0;
		};

		/**
		 * \brief Templated container to hold a value of type T
		 *
		 * \tparam T Type of the stored value
		 */
		template <typename T>
		class AnyHolder : public AbstractAnyHolder
		{
		public:

			AnyHolder() = delete;

			explicit AnyHolder(const T& value) :
				mValue(value)
			{
				/// Nothing
			}

			explicit AnyHolder(T&& value) :
				mValue(std::move(value))
			{
				/// Nothing
			}

			template <typename... Args>
			explicit AnyHolder(InPlaceTag, Args&&... args) :
				mValue(std::forward<Args>(args)...)
			{
				/// Nothing
			}

			T& Get()
			{
				return mValue;
			}

			const std::type_info& GetTypeInfo() const override
			{
				return typeid(T);
			}

		private:

			T mValue;
		};

		UniquePtr<AbstractAnyHolder> mHolder;
	};
}

#endif // COCKTAIL_CORE_UTILITY_ANY_HPP