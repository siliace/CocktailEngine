#ifndef COCKTAIL_CORE_UTILITY_OPTIONAL_HPP
#define COCKTAIL_CORE_UTILITY_OPTIONAL_HPP

#include <stdexcept>
#include <utility>

#define COCKTAIL_DECLARE_TAG(__Name) struct __Name##Tag{}; constexpr __Name##Tag __Name;

namespace Ck
{
	COCKTAIL_DECLARE_TAG(InPlace)

	/**
	 * \brief 
	 */
	class EmptyOptionalException : public std::runtime_error
	{
	public:

		/**
		 * \brief 
		 */
		EmptyOptionalException() :
			std::runtime_error("Optional has been empty")
		{
			/// Nothing
		}
	};

	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Optional
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static Optional Empty()
		{
			return {};
		}

		/**
		 * \brief
		 * \param value
		 * \return
		 */
		static Optional Of(T&& value)
		{
			return Optional<T>(std::forward<T>(value));
		}

		/**
		 * \brief 
		 * \param value 
		 * \return 
		 */
		static Optional Of(const T& value)
		{
			return Optional(value);
		}

		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 * \return 
		 */
		template <typename... Args>
		static Optional Of(InPlaceTag, Args&&... args)
		{
			return Optional(InPlace, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 */
		Optional() :
			mEmpty(true),
			mStorage{}
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param other 
		 */
		Optional(const Optional& other) :
			mEmpty(other.mEmpty),
			mStorage(other.mStorage)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param other 
		 */
		Optional(Optional&& other) noexcept :
			mEmpty(std::exchange(other.mEmpty, true)),
			mStorage{}
		{
			mStorage = std::move(other.mStorage);
		}

		~Optional()
		{
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Optional& operator=(const Optional& other)
		{
			if (!mEmpty)
				Destroy();

			mEmpty = other.mEmpty;
			if (!mEmpty)
				new (&mStorage) T(other.Get());

			return *this;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Optional& operator=(Optional&& other) noexcept
		{
			if (!mEmpty)
				Destroy();

			if (!other.mEmpty)
			{
				new (&mStorage) T(std::move(other.Get()));
				mEmpty = std::exchange(other.mEmpty, true);
			}

			return *this;
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEmpty() const
		{
			return mEmpty;
		}

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param mapper 
		 * \return 
		 */
		template <typename Callable>
		auto Map(Callable&& mapper) const -> Optional<decltype(mapper(T{}))>
		{
			using U = decltype(mapper(T{}));

			if (mEmpty)
				return Optional<U>::Empty();

			return Optional<U>::Of(mapper(GetValue()));
		}

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param callable 
		 */
		template <typename Callable>
		void Then(Callable&& callable) const
		{
			if (!mEmpty)
				callable(GetValue());
		}

		/**
		 * \brief
		 * \return
		 */
		T& Get()
		{
			return GetOrThrow<EmptyOptionalException>();
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& Get() const
		{
			return GetOrThrow<EmptyOptionalException>();
		}

		/**
		 * \brief 
		 * \param fallback 
		 * \return 
		 */
		T& GetOr(T& fallback)
		{
			return !mEmpty ? Get() : fallback;
		}

		/**
		 * \brief 
		 * \param fallback 
		 * \return 
		 */
		const T& GetOr(const T& fallback) const
		{
			return !mEmpty ? Get() : fallback;
		}

		/**
		 * \brief
		 * \param fallback
		 * \return
		 */
		template <typename Callable>
		auto GetOrElse(Callable&& fallback) -> decltype(fallback())
		{
			return !mEmpty ? Get() : fallback();
		}

		/**
		 * \brief
		 * \param fallback
		 * \return
		 */
		template <typename Callable>
		auto GetOrElse(Callable&& fallback) const -> decltype(fallback())
		{
			return !mEmpty ? Get() : fallback();
		}

		/**
		 * \brief 
		 * \tparam E 
		 * \tparam Args 
		 * \param args 
		 * \return 
		 */
		template <typename E, typename... Args>
		T& GetOrThrow(Args&&... args)
		{
			if (mEmpty)
				throw E(std::forward<Args>(args)...);

			return GetValue();
		}

		/**
		 * \brief 
		 * \tparam E
		 * \tparam Args 
		 * \param args 
		 * \return 
		 */
		template <typename E, typename... Args>
		const T& GetOrThrow(Args&&... args) const
		{
			if (mEmpty)
				throw E(std::forward<Args>(args)...);

			return GetValue();
		}

	private:

		/**
		 * \brief
		 * \param value
		 * \return
		 */
		explicit Optional(T&& value) :
			mEmpty(false)
		{
			new (&mStorage) T(std::forward<T>(value));
		}

		/**
		 * \brief
		 * \param value
		 * \return
		 */
		explicit Optional(const T& value) :
			mEmpty(false)
		{
			new (&mStorage) T(value);
		}
		
		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 */
		template <typename... Args>
		explicit Optional(InPlaceTag, Args&&... args) :
			mEmpty(false)
		{
			new (&mStorage) T(std::forward<Args>(args)...);
		}

		/**
		 * \brief 
		 */
		void Destroy()
		{
			std::destroy_at(reinterpret_cast<T*>(&mStorage));
			mEmpty = true;
		}

		/**
		 * \brief 
		 * \return 
		 */
		T& GetValue()
		{
			return *reinterpret_cast<T*>(&mStorage);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& GetValue() const
		{
			return *reinterpret_cast<const T*>(&mStorage);
		}

		bool mEmpty;
		std::aligned_storage_t<sizeof(T), alignof(T)> mStorage;
	};
}

#endif // COCKTAIL_CORE_UTILITY_OPTIONAL_HPP
