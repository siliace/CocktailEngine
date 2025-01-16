#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEBINDING_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEBINDING_HPP

#include <Cocktail/Core/Application/Detail/ServiceBindingBase.hpp>

namespace Ck::Detail
{
	template <typename T>
	class ServiceBinding : public ServiceBindingBase
	{
	public:
	
		/**
		 * \brief
		 * \return
		 */
		virtual T* Resolve() = 0;
	
		/**
		 * \brief
		 * \return
		 */
		const std::type_info& GetResolvedType() const override
		{
			return typeid(T);
		}
	};
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEBINDING_HPP
