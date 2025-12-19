#ifndef COCKTAIL_CORE_APPLICATION_SERVICEFACADE_HPP
#define COCKTAIL_CORE_APPLICATION_SERVICEFACADE_HPP

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class ServiceFacade : public Detail::ServiceFacadeBase
	{
	public:

		/**
		 * \brief 
		 * \param force 
		 * \param cached 
		 * \return 
		 */
		static T* ResolveFacadeInstance(bool force = false, bool cached = true)
		{
		    Application* application = GetApplicationFacade();
		    assert(application != nullptr && "Trying to access to a facade before booting or after termination");

			if (!force && sInstance)
				return sInstance;

			T* instance = application->Resolve<T>();
			if (cached)
				sInstance = instance;

			return instance;
		}

	private:

		static T* sInstance;
	};

	template <typename T>
	inline T* ServiceFacade<T>::sInstance;
}

#endif // COCKTAIL_CORE_APPLICATION_SERVICEFACADE_HPP
