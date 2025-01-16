#ifndef COCKTAIL_GRAPHIC_GRAPHICSERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_GRAPHICSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	class GraphicServiceProvider : public Extends<GraphicServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit GraphicServiceProvider(Ref<Application> application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_GRAPHIC_GRAPHICSERVICEPROVIDER_HPP
