#ifndef COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP
#define COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
    /**
     * \brief Service provider responsible for image-related services
     *
     * This service provider registers and initializes all services related to
     * image handling, such as image loading, decoding, format conversion and
     * in-memory representation.
     *
     * The ImageServiceProvider ensures that image-related systems are properly
     * registered during the application setup phase and fully initialized
     * during the boot phase.
     */
	class COCKTAIL_CORE_API ImageServiceProvider : public ServiceProvider
	{
	public:

	    /**
         * \brief Constructor
         *
         * Creates an image service provider associated with the given application.
         *
         * \param application The application in which image services will be registered
         */
		explicit ImageServiceProvider(Application* application);

	protected:

	    /**
         * \brief Registers image-related services
         *
         * This function is responsible for registering all image-related services
         * into the application, such as image loaders, decoders, format utilities
         * or resource managers.
         *
         * This phase should only declare and bind services, without performing
         * heavy initialization or relying on other services being fully booted.
         *
         * \param application Target application
         */
		void DoRegister(Application* application) override;

	    /**
         * \brief Boots image-related services
         *
         * Performs initialization steps that require all image services (and
         * potentially other application services) to be fully registered.
         *
         * Typical responsibilities include:
         * - final initialization of image decoders
         * - registration of default image formats
         * - validation of image pipelines
         *
         * \param application Target application
         */
		void DoBoot(Application* application) override;
	};
}

#endif // COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP
