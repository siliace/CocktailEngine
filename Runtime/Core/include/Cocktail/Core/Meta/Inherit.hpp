#ifndef COCKTAIL_CORE_META_INHERIT_HPP
#define COCKTAIL_CORE_META_INHERIT_HPP

#include <Cocktail/Core/Meta/Extends.hpp>
#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 * \tparam Parent 
	 * \tparam Interfaces 
	 */
	template <typename T, typename Parent, typename... Interfaces>
	class Inherit : public Extends<T, Parent>, public Implements<T, Interfaces...>
	{
	public:

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
		static T* Cast(U* other)
		{
			return Extends<T, Parent>::Cast(other);
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
			return Extends<T, Parent>::Cast(other);
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
			return Extends<T, Parent>::Cast(other);
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
			return Extends<T, Parent>::Cast(other);
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
			return Extends<T, Parent>::TryCast(other);
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
			return Extends<T, Parent>::TryCast(other);
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
			return Extends<T, Parent>::TryCast(other);
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
			return Extends<T, Parent>::TryCast(other);
		}

		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 */
		template <typename... Args>
		explicit Inherit(Args&&... args) :
			Extends<T, Parent>(std::forward<Args>(args)...)
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
			return Extends<T, Parent>::HasTypeInfo(typeInfo) || Implements<T, Interfaces...>::HasTypeInfo(typeInfo);
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
		 * \brief 
		 * \return 
		 */
		Ref<T> Self()
		{
			return Extends<T, Parent>::Self();
		}

		/**
		 * \brief 
		 * \return 
		 */
		WeakRef<T> WeakSelf()
		{
			return Extends<T, Parent>::WeakSelf();
		}

		/**
		 * \brief 
		 * \return 
		 */
		RefCounter* GetRefCounter() const override
		{
			return Object::GetRefCounter();
		}
	};
}

#endif // COCKTAIL_CORE_META_INHERIT_HPP
