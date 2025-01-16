#ifndef COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP
#define COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API ImageServiceProvider : public Extends<ImageServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit ImageServiceProvider(const Ref<Application>& application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(const Ref<Application>& application) override;

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP
