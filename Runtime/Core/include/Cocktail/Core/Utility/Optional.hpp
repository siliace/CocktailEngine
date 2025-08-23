#ifndef COCKTAIL_CORE_UTILITY_OPTIONAL_HPP
#define COCKTAIL_CORE_UTILITY_OPTIONAL_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Exception.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION(EmptyOptionalException, "Optional has been empty");

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
			mEmpty(true)
		{
			*this = other;
		}

		/**
		 * \brief 
		 * \param other 
		 */
		Optional(Optional&& other) noexcept :
			mEmpty(true)
		{
			*this = std::move(other);
		}

		/**
		 * \brief 
		 */
		~Optional()
		{
			Destroy();
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Optional& operator=(const Optional& other)
		{
			if (this == &other)
				return *this;

			Destroy();

			mEmpty = other.mEmpty;
			if (!mEmpty)
				new (&mStorage) T(other.GetValue());

			return *this;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Optional& operator=(Optional&& other) noexcept
		{
			if (this == &other)
				return *this;

			Destroy();

			mEmpty = std::exchange(other.mEmpty, true);
			if (!mEmpty)
				new (&mStorage) T(std::move(other.GetValue()));

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
		Optional<T> Then(Callable&& callable) const
		{
			if (!mEmpty)
				callable(GetValue());

			return *this;
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
			if (mEmpty)
				return;

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

	template <typename T>
	class Optional<T&>
	{
		template <typename>
		friend class Optional;

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
		static Optional Of(T& value)
		{
			return Optional<T&>(&value);
		}

		/**
		 * \brief
		 */
		Optional() :
			mValue(nullptr)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param other
		 */
		Optional(const Optional& other) :
			mValue(other.mValue)
		{
			/// Nothing
		}

		template <typename U,
			typename = std::enable_if_t<
				std::is_same_v<
					std::remove_const_t<T>, U
				>
			>
		>
		Optional(const Optional<U&>& other) :
			mValue(other.mValue)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param other
		 */
		Optional(Optional&& other) noexcept
		{
			mValue = std::exchange(other.mValue, nullptr);
		}

		/**
		 * \brief
		 * \param other
		 * \return
		 */
		Optional& operator=(const Optional& other)
		{
			if (this == &other)
				return *this;

			mValue = other.mValue;
			return *this;
		}

		template <typename U,
			typename = std::enable_if_t<
				std::is_same_v<
					std::remove_const_t<T>, U
				>
			>
		>
		Optional& operator=(const Optional<U&>& other)
		{
			mValue = other.mValue;
			return *this;
		}

		/**
		 * \brief
		 * \param other
		 * \return
		 */
		Optional& operator=(Optional&& other) noexcept
		{
			mValue = std::exchange(other.mValue, nullptr);
			return *this;
		}

		/**
		 * \brief
		 * \return
		 */
		bool IsEmpty() const
		{
			return mValue == nullptr;
		}

		/**
		 * \brief
		 * \tparam Callable
		 * \param mapper
		 * \return
		 */
		template <typename Callable>
		auto Map(Callable&& mapper) const -> Optional<decltype(mapper(T{})) >
		{
			using U = decltype(mapper(T{}));

			if (mValue)
				return Optional<U>::Empty();

			return Optional<U>::Of(mapper(*mValue));
		}

		/**
		 * \brief
		 * \tparam Callable
		 * \param callable
		 */
		template <typename Callable>
		Optional<T&> Then(Callable&& callable) const
		{
			if (mValue)
				callable(*mValue);

			return *this;
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
			return mValue ? Get() : fallback;
		}

		/**
		 * \brief
		 * \param fallback
		 * \return
		 */
		const T& GetOr(const T& fallback) const
		{
			return mValue ? Get() : fallback;
		}

		/**
		 * \brief
		 * \param fallback
		 * \return
		 */
		template <typename Callable>
		auto GetOrElse(Callable&& fallback) -> decltype(fallback())
		{
			return mValue ? Get() : fallback();
		}

		/**
		 * \brief
		 * \param fallback
		 * \return
		 */
		template <typename Callable>
		auto GetOrElse(Callable&& fallback) const -> decltype(fallback())
		{
			return mValue ? Get() : fallback();
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
			if (!mValue)
				throw E(std::forward<Args>(args)...);

			return *mValue;
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
			if (!mValue)
				throw E(std::forward<Args>(args)...);

			return *mValue;
		}

	private:

		/**
		 * \brief Constructor
		 * \param value 
		 */
		explicit Optional(T* value) :
			mValue(value)
		{
			/// Nothing
		}

		T* mValue;
	};
}

#endif // COCKTAIL_CORE_UTILITY_OPTIONAL_HPP
