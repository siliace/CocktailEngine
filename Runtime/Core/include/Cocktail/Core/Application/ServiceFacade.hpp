#ifndef COCKTAIL_CORE_APPLICATION_SERVICEFACADE_HPP
#define COCKTAIL_CORE_APPLICATION_SERVICEFACADE_HPP

#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class ServiceFacade : Detail::ServiceFacadeBase
	{
	public:

		/**
		 * \brief 
		 * \param force 
		 * \param cached 
		 * \return 
		 */
		static Ref<T> ResolveFacadeInstance(bool force = false, bool cached = true)
		{
			if (!force && sInstance)
				return sInstance;

			Ref<T> instance = GetApplicationFacade()->Resolve<T>();
			if (cached)
				sInstance = instance;

			return instance;
		}

	private:

		static Ref<T> sInstance;
	};

	template <typename T>
	inline Ref<T> ServiceFacade<T>::sInstance;
}

#endif // COCKTAIL_CORE_APPLICATION_SERVICEFACADE_HPP
