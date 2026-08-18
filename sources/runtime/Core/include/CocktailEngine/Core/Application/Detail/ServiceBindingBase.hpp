#ifndef COCKTAILENGINE_CORE_APPLICATION_DETAIL_SERVICEBINDINGBASE_HPP
#define COCKTAILENGINE_CORE_APPLICATION_DETAIL_SERVICEBINDINGBASE_HPP

#include <typeinfo>

namespace Ck::Detail
{
	class ServiceBindingBase
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~ServiceBindingBase() = default;

		/**
		 * \brief 
		 * \return 
		 */
		virtual bool IsCallable() const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual const std::type_info& GetResolvedType() const = 0;
	};
}

#endif // COCKTAILENGINE_CORE_APPLICATION_DETAIL_SERVICEBINDINGBASE_HPP
