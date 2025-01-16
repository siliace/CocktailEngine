#ifndef COCKTAIL_CORE_META_EXTENDS_HPP
#define COCKTAIL_CORE_META_EXTENDS_HPP

#include <typeinfo>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Memory/Ref.hpp>
#include <Cocktail/Core/Memory/WeakRef.hpp>
#include <Cocktail/Core/Memory/Detail/StorageRefCounter.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 * \tparam Parent 
	 */
	template <typename T, typename Parent, typename = std::enable_if_t<IsObject<Parent>>>
	class Extends : public Parent
	{
	public:

		/**
		 * \brief Utility type of access to the parent class
		 */
		using Super = Extends<T, Parent>;

		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 * \return 
		 */
		template <typename... Args>
		static Ref<T> New(Args&&... args)
		{
			Detail::StorageRefCounter<T>* refCounter = new Detail::StorageRefCounter<T>(std::forward<Args>(args)...);

			T* pointer = refCounter->GetPointer();
			Object::AssignRefCounter(pointer, refCounter);

			return Ref<T>(pointer, refCounter);
		}

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static T* Cast(U* other)
		{
			if (!other)
				return nullptr;

			if (!other->HasTypeInfo(typeid(T)))
				throw std::bad_cast();

			return static_cast<T*>(other);
		}

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static const T* Cast(const U* other)
		{
			if (!other)
				return nullptr;

			if (!other->HasTypeInfo(typeid(T)))
				throw std::bad_cast();

			return static_cast<const T*>(other);
		}

		/**
		 * \brief 
		 * \tparam U 
		 * \param other 
		 * \return 
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static Ref<T> Cast(const Ref<U>& other)
		{
			if (!other)
				return nullptr;

			if (!other->HasTypeInfo(typeid(T)))
				throw std::bad_cast();

			RefCounter* refCounter = other.GetRefCounter();
			refCounter->Acquire();

			return Ref<T>(static_cast<T*>(other.Get()), refCounter);
		}

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static WeakRef<T> Cast(const WeakRef<U>& other)
		{
			if (!other)
				return nullptr;

			if (!other->HasTypeInfo(typeid(T)))
				throw std::bad_cast();

			RefCounter* refCounter = other.GetRefCounter();
			refCounter->WeakAcquire();

			return WeakRef<T>(static_cast<T*>(other.Get()), refCounter);
		}

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static T* TryCast(U* other)
		{
			if (!other || !other->HasTypeInfo(typeid(T)))
				return nullptr;

			return static_cast<T*>(other);
		}

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static const T* TryCast(const U* other)
		{
			if (!other || !other->HasTypeInfo(typeid(T)))
				return nullptr;

			return static_cast<const T*>(other);
		}

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static Ref<T> TryCast(const Ref<U>& other)
		{
			if (!other || !other->HasTypeInfo(typeid(T)))
				return nullptr;

			RefCounter* refCounter = other.GetRefCounter();
			refCounter->Acquire();

			return Ref<T>(static_cast<T*>(other.Get()), refCounter);
		}

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static WeakRef<T> TryCast(const WeakRef<U>& other)
		{
			if (!other || !other->HasTypeInfo(typeid(T)))
				return nullptr;

			RefCounter* refCounter = other.GetRefCounter();
			refCounter->WeakAcquire();

			return WeakRef<T>(static_cast<T*>(other.Get()), refCounter);
		}

		/**
		 * \brief 
		 */
		Extends() = default;

		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 */
		template <typename... Args>
		explicit Extends(Args&&... args) :
			Parent(std::forward<Args>(args)...)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param typeInfo 
		 * \return 
		 */
		bool HasTypeInfo(const std::type_info& typeInfo) const override
		{
			return typeid(T) == typeInfo || Parent::HasTypeInfo(typeInfo);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const std::type_info& GetTypeInfo() const override
		{
			return typeid(T);
		}

	protected:
		
		/**
		 * \brief Create a Ref from this
		 * \return 
		 */
		Ref<T> Self()
		{
			RefCounter* refCounter = Object::GetRefCounter();

			refCounter->Acquire();
			return Ref<T>(static_cast<T*>(this), refCounter);
		}

		/**
		 * \brief Create a WeakRef from this
		 * \return 
		 */
		WeakRef<T> WeakSelf()
		{
			RefCounter* refCounter = Object::GetRefCounter();

			refCounter->WeakAcquire();
			return WeakRef<T>(static_cast<T*>(this), refCounter);
		}
	};
}

#endif // COCKTAIL_CORE_META_EXTENDS_HPP
