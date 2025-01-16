#ifndef COCKTAIL_CORE_META_INTERFACE_HPP
#define COCKTAIL_CORE_META_INTERFACE_HPP

#include <typeinfo>

namespace Ck
{
	/**
	 * \brief
	 */
	class Interface
	{
	public:

		/**
		 * \brief
		 */
		virtual ~Interface() = default;

		/**
		 * \brief
		 * \param typeInfo
		 * \return
		 */
		virtual bool HasTypeInfo(const std::type_info& typeInfo) const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual const std::type_info& GetTypeInfo() const = 0;

	protected:

		/**
		 * \brief
		 * \return
		 */
		virtual RefCounter* GetRefCounter() const = 0;
	};
}

#endif // COCKTAIL_CORE_META_INTERFACE_HPP
