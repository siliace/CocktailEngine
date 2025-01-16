#ifndef COCKTAIL_CORE_MEMORY_REF_HPP
#define COCKTAIL_CORE_MEMORY_REF_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Memory/RefCounter.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Ref
	{
	public:
		
		/**
		 * \brief 
		 */
		Ref() :
			Ref(nullptr, nullptr)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 */
		Ref(std::nullptr_t) :
			Ref(nullptr, nullptr)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param pointer
		 * \param refCounter
		 */
		Ref(T* pointer, RefCounter* refCounter) :
			mPointer(pointer),
			mRefCounter(refCounter)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param rhs 
		 */
		Ref(const Ref& rhs) :
			Ref(nullptr, nullptr)
		{
			Reset(rhs.mPointer, rhs.mRefCounter);
		}

		/**
		 * \brief
		 * \param rhs
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		Ref(const Ref<U>& rhs) :
			Ref(nullptr, nullptr)
		{
			Reset(static_cast<T*>(rhs.Get()), rhs.GetRefCounter());
		}

		/**
		 * \brief 
		 * \param other 
		 */
		Ref(Ref&& other) noexcept :
			Ref(nullptr, nullptr)
		{
			mPointer = std::exchange(other.mPointer, nullptr);
			mRefCounter = std::exchange(other.mRefCounter, nullptr);
		}

		/**
		 * \brief
		 * \param other
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		Ref(Ref<U>&& other) noexcept :
			Ref(nullptr, nullptr)
		{
			mPointer = std::exchange(other.mPointer, nullptr);
			mRefCounter = std::exchange(other.mRefCounter, nullptr);
		}

		/**
		 * \brief 
		 */
		~Ref()
		{
			Reset(nullptr);
		}

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		Ref& operator=(const Ref& rhs)
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
		Ref& operator=(const Ref<U>& rhs)
		{
			Reset(static_cast<T*>(rhs.Get()), rhs.GetRefCounter());
			return *this;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Ref& operator=(Ref&& other) noexcept
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
		Ref& operator=(Ref<U>&& other) noexcept
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
				mRefCounter->Release();

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
				mRefCounter->Release();

			mPointer = pointer;
			mRefCounter = refCounter;

			if (mRefCounter)
				mRefCounter->Acquire();
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
		bool IsUnique() const
		{
			return mRefCounter ? mRefCounter->GetCounter() == 1 : true;
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

		template <typename>
		friend class Ref;

		T* mPointer;
		RefCounter* mRefCounter;
	};

	template <typename T>
	bool operator==(std::nullptr_t, const Ref<T>& rhs)
	{
		return nullptr == rhs.Get();
	}

	template <typename T>
	bool operator==(const Ref<T>& lhs, std::nullptr_t)
	{
		return lhs.Get() == nullptr;
	}

	template <typename T>
	bool operator==(const T* lhs, const Ref<T>& rhs)
	{
		return lhs == rhs.Get();
	}

	template <typename T>
	bool operator==(const Ref<T>& lhs, const T* rhs)
	{
		return lhs.Get() == rhs;
	}

	template <typename T>
	bool operator==(const Ref<T>& lhs, const Ref<T>& rhs)
	{
		return lhs.Get() == rhs.Get();
	}

	template <typename T>
	bool operator!=(std::nullptr_t, const Ref<T>& rhs)
	{
		return nullptr != rhs.Get();
	}

	template <typename T>
	bool operator!=(const Ref<T>& lhs, std::nullptr_t)
	{
		return lhs.Get() != nullptr;
	}

	template <typename T>
	bool operator!=(const T* lhs, const Ref<T>& rhs)
	{
		return lhs != rhs.Get();
	}

	template <typename T>
	bool operator!=(const Ref<T>& lhs, const T* rhs)
	{
		return lhs.Get() != rhs;
	}

	template <typename T>
	bool operator!=(const Ref<T>& lhs, const Ref<T>& rhs)
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
			std::is_base_of_v<Object, T> &&
			std::is_base_of_v<Object, U>
		>
	>
	bool InstanceOf(const Ref<U>& ref)
	{
		return ref->HasTypeInfo(typeid(T));
	}
}

namespace std
{
	template <typename T>
	struct hash<Ck::Ref<T>>
	{
		size_t operator()(const Ck::Ref<T>& ref) const noexcept
		{
			return std::hash<T*>()(ref.Get());
		}
	};
}

#endif // COCKTAIL_CORE_MEMORY_REF_HPP
