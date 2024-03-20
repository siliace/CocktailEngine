#ifndef COCKTAIL_CORE_META_IMPLEMENTS_HPP
#define COCKTAIL_CORE_META_IMPLEMENTS_HPP

#include <Cocktail/Core/Memory/Ref.hpp>
#include <Cocktail/Core/Meta/Interface.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 * \tparam Interfaces 
	 */
	template <typename T, typename... Interfaces>
	class Implements : public Interfaces...
	{
	public:

		static_assert(sizeof...(Interfaces) > 0, "Cannot inherit of zero interfaces");

		/**
		 * \brief 
		 * \tparam U 
		 * \param other 
		 * \return 
		 */
		template <typename U>
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
		 * \param typeInfo
		 * \return
		 */
		bool HasTypeInfo(const std::type_info& typeInfo) const override
		{
			for (bool hasTypeInfo : { Interfaces::HasTypeInfo(typeInfo)... })
			{
				if (hasTypeInfo)
					return true;
			}

			return typeid(T) == typeInfo;
		}

		/**
		 * \brief
		 * \return
		 */
		const std::type_info& GetTypeInfo() const override
		{
			return typeid(T);
		}
	};

	template <typename T>
	class Implements<T, Interface> : public Interface
	{
	public:

		/**
		 * \brief
		 * \tparam U
		 * \param other
		 * \return
		 */
		template <typename U>
		static Ref<T> Cast(const Ref<U>& other)
		{
			if (!other)
				return nullptr;

			if (!other->HasTypeInfo(typeid(T)))
				throw std::bad_cast();

			RefCounter* refCounter = other.GetRefCounter();
			refCounter->Acquire();

			return Ref<T>(reinterpret_cast<T*>(other.Get()), refCounter);
		}

		/**
		 * \brief
		 * \param typeInfo
		 * \return
		 */
		bool HasTypeInfo(const std::type_info& typeInfo) const override
		{
			return typeid(T) == typeInfo;
		}

		/**
		 * \brief
		 * \return
		 */
		const std::type_info& GetTypeInfo() const override
		{
			return typeid(T);
		}
	};
}

#endif // COCKTAIL_CORE_META_IMPLEMENTS_HPP
