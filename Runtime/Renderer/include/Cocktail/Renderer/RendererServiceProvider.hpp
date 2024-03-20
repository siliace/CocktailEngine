#ifndef COCKTAIL_RENDERER_RENDERERSERVICEPROVIDER_HPP
#define COCKTAIL_RENDERER_RENDERERSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

#include <Cocktail/Renderer/Export.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	class COCKTAIL_RENDERER_API RendererServiceProvider : public Extends<RendererServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit RendererServiceProvider(const Ref<Application>& application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_RENDERER_RENDERERSERVICEPROVIDER_HPP
