#ifndef COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP
#define COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API ImageServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit ImageServiceProvider(Application* application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(Application* application) override;

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(Application* application) override;
	};
}

#endif // COCKTAIL_CORE_IMAGE_IMAGESERVICEPROVIDER_HPP
