#ifndef COCKTAIL_CORE_MEMORY_WEAKREF_HPP
#define COCKTAIL_CORE_MEMORY_WEAKREF_HPP

#include <Cocktail/Core/Memory/Ref.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class WeakRef
	{
	public:

		/**
		 * \brief
		 */
		WeakRef() :
			WeakRef(nullptr, nullptr)
		{
			/// Nothing
		}

		/**
		 * \brief
		 */
		WeakRef(std::nullptr_t) :
			WeakRef(nullptr, nullptr)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param pointer 
		 * \param refCounter 
		 */
		WeakRef(T* pointer, RefCounter* refCounter) :
			mPointer(pointer),
			mRefCounter(refCounter)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param rhs
		 */
		WeakRef(const Ref<T>& rhs) :
			WeakRef(nullptr, nullptr)
		{
			Reset(rhs.Get(), rhs.GetRefCounter());
		}

		/**
		 * \brief
		 * \param rhs
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		WeakRef(const Ref<U>& rhs) :
			WeakRef(nullptr, nullptr)
		{
			Reset(static_cast<T*>(rhs.Get()), rhs.GetRefCounter());
		}

		/**
		 * \brief
		 * \param rhs
		 */
		WeakRef(const WeakRef& rhs) :
			WeakRef(nullptr, nullptr)
		{
			Reset(rhs.mPointer, rhs.mRefCounter);
		}

		/**
		 * \brief
		 * \param rhs
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		WeakRef(const WeakRef<U>& rhs) :
			WeakRef(nullptr, nullptr)
		{
			Reset(static_cast<T*>(rhs.Get()), rhs.GetRefCounter());
		}

		/**
		 * \brief
		 * \param other
		 */
		WeakRef(WeakRef&& other) noexcept :
			WeakRef(nullptr, nullptr)
		{
			mPointer = std::exchange(other.mPointer, nullptr);
			mRefCounter = std::exchange(other.mRefCounter, nullptr);
		}

		/**
		 * \brief
		 * \param other
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		WeakRef(WeakRef<U>&& other) noexcept :
			WeakRef(nullptr, nullptr)
		{
			mPointer = std::exchange(other.mPointer, nullptr);
			mRefCounter = std::exchange(other.mRefCounter, nullptr);
		}

		/**
		 * \brief 
		 */
		~WeakRef()
		{
			Reset(nullptr);
		}

		WeakRef& operator=(std::nullptr_t)
		{
			Reset(nullptr, nullptr);
			return *this;
		}

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		WeakRef& operator=(const Ref<T>& rhs)
		{
			Reset(rhs.Get(), rhs.GetRefCounter());
			return *this;
		}

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		WeakRef& operator=(const Ref<U>& rhs)
		{
			Reset(static_cast<T*>(rhs.Get()), rhs.GetRefCounter());
			return *this;
		}

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		WeakRef& operator=(const WeakRef& rhs)
		{
			if (&rhs == this)
				return *this;

			Reset(rhs.mPointer, rhs.mRefCounter);
			return *this;
		}

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		WeakRef& operator=(const WeakRef<U>& rhs)
		{
			Reset(static_cast<T*>(rhs.Get()), rhs.GetRefCounter());
			return *this;
		}

		/**
		 * \brief
		 * \param other
		 * \return
		 */
		WeakRef& operator=(WeakRef&& other) noexcept
		{
			Reset(nullptr);

			mPointer = std::exchange(other.mPointer, nullptr);
			mRefCounter = std::exchange(other.mRefCounter, nullptr);

			return *this;
		}

		/**
		 * \brief
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		WeakRef& operator=(WeakRef<U>&& other) noexcept
		{
			Reset(nullptr);

			mPointer = std::exchange(other.mPointer, nullptr);
			mRefCounter = std::exchange(other.mRefCounter, nullptr);

			return *this;
		}

		/**
		 * \brief
		 */
		void Reset(std::nullptr_t)
		{
			if (mRefCounter)
				mRefCounter->WeakRelease();

			mPointer = nullptr;
			mRefCounter = nullptr;
		}

		/**
		 * \brief
		 * \param pointer
		 * \param refCounter
		 */
		void Reset(T* pointer, RefCounter* refCounter)
		{
			if (mRefCounter)
				mRefCounter->WeakRelease();

			mPointer = pointer;
			mRefCounter = refCounter;

			if (mRefCounter)
				mRefCounter->WeakAcquire();
		}

		/**
		 * \brief
		 * \return
		 */
		T* Get() const
		{
			return mPointer;
		}

		/**
		 * \brief
		 * \return
		 */
		RefCounter* GetRefCounter() const
		{
			return mRefCounter;
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsExpired() const
		{
			return mRefCounter == nullptr || mRefCounter->GetCounter() == 0;
		}

		/**
		 * \brief 
		 * \return 
		 */
		Ref<T> ToStrong() const
		{
			if (IsExpired())
				return nullptr;

			mRefCounter->Acquire();
			return Ref<T>(mPointer, mRefCounter);
		}

		/**
		 * \brief
		 * \return
		 */
		T* operator->() const
		{
			return mPointer;
		}

		/**
		 * \brief
		 * \return
		 */
		T& operator*() const
		{
			return *mPointer;
		}

		/**
		 * \brief
		 */
		operator bool() const
		{
			return mPointer != nullptr;
		}

	private:

		T* mPointer;
		RefCounter* mRefCounter;
	};

	template <typename T>
	bool operator==(std::nullptr_t, const WeakRef<T>& rhs)
	{
		return nullptr == rhs.Get();
	}

	template <typename T>
	bool operator==(const WeakRef<T>& lhs, std::nullptr_t)
	{
		return lhs.Get() == nullptr;
	}

	template <typename T>
	bool operator==(const WeakRef<T>& lhs, const WeakRef<T>& rhs)
	{
		return lhs.Get() == rhs.Get();
	}

	template <typename T>
	bool operator!=(std::nullptr_t, const WeakRef<T>& rhs)
	{
		return nullptr != rhs.Get();
	}

	template <typename T>
	bool operator!=(const WeakRef<T>& lhs, std::nullptr_t)
	{
		return lhs.Get() != nullptr;
	}

	template <typename T>
	bool operator!=(const WeakRef<T>& lhs, const WeakRef<T>& rhs)
	{
		return lhs.Get() != rhs.Get();
	}

	/**
	 * \brief
	 * \tparam T
	 * \tparam U
	 * \param ref
	 * \return
	 */
	template <
		typename T,
		typename U,
		typename = std::enable_if_t<
		std::is_base_of_v<Object, T>&&
		std::is_base_of_v<Object, U>
		>
	>
	bool InstanceOf(const WeakRef<U>& ref)
	{
		return ref->HasTypeInfo(typeid(T));
	}
}

namespace std
{
	template <typename T>
	struct hash<Ck::WeakRef<T>>
	{
		size_t operator()(const Ck::WeakRef<T>& ref) const noexcept
		{
			return std::hash<T*>()(ref.Get());
		}
	};
}

#endif // COCKTAIL_CORE_MEMORY_WEAKREF_HPP
