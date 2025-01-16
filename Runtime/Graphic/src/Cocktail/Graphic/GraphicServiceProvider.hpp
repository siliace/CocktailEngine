#ifndef COCKTAIL_GRAPHIC_GRAPHICSERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_GRAPHICSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	class GraphicServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit GraphicServiceProvider(Application* application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(Application* application) override;
	};
}

#endif // COCKTAIL_GRAPHIC_GRAPHICSERVICEPROVIDER_HPP
