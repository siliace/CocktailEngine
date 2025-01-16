#ifndef COCKTAIL_RENDERER_RENDERERSERVICEPROVIDER_HPP
#define COCKTAIL_RENDERER_RENDERERSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

#include <Cocktail/Renderer/Export.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	class COCKTAIL_RENDERER_API RendererServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit RendererServiceProvider(Application* application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(Application* application) override;
	};
}

#endif // COCKTAIL_RENDERER_RENDERERSERVICEPROVIDER_HPP
