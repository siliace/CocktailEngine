#ifndef COCKTAILENGINE_CORE_APPLICATION_SERVICEFACADE_HPP
#define COCKTAILENGINE_CORE_APPLICATION_SERVICEFACADE_HPP

#include <CocktailEngine/Core/Application/Application.hpp>

namespace Ck
{
    /**
     * \brief Base class for static facades that expose application services
     *
     * A facade resolves a service from the application made active during the
     * pre-boot phase. Resolved services can be cached for subsequent facade calls.
     * Accessing a facade before boot or after termination is invalid.
     *
     * \tparam T Service type exposed by the facade
     */
    template <typename T>
    class ServiceFacade : public Detail::ServiceFacadeBase
    {
    public:

        /**
         * \brief Resolves the service instance exposed by this facade
         *
         * \param force When true, bypasses a previously cached instance and resolves the service again
         * \param cached When resolving a fresh instance, determines whether to store it in the cache
         * \return Resolved service instance, or nullptr when no matching service is registered
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

        static T* sInstance; /*!< Cached service instance for this facade specialization. */
    };

    template <typename T>
    inline T* ServiceFacade<T>::sInstance;
}

#endif // COCKTAILENGINE_CORE_APPLICATION_SERVICEFACADE_HPP
