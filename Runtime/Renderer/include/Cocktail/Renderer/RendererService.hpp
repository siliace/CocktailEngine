#ifndef COCKTAIL_RENDERER_RENDERERSERVICE_HPP
#define COCKTAIL_RENDERER_RENDERERSERVICE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Export.hpp>
#include <Cocktail/Renderer/GraphicApi.hpp>
#include <Cocktail/Renderer/RenderDeviceFactory.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief
	 */
	class COCKTAIL_RENDERER_API RendererService : public Extends<RendererService, Object>
	{
	public:

		/**
		 * \brief 
		 * \param api 
		 * \param enableDebug 
		 * \return 
		 */
		Ref<RenderDevice> CreateRenderDevice(GraphicApi api, bool enableDebug) const;

		/**
		 * \brief 
		 * \param api 
		 * \param factory 
		 */
		void SetFactory(GraphicApi api, const RenderDeviceFactory& factory);

		/**
		 * \brief
		 * \param api
		 * \return
		 */
		bool IsApiSupported(GraphicApi api) const;

	private:

		EnumMap<GraphicApi, RenderDeviceFactory> mFactories;
	};
}

#endif // COCKTAIL_RENDERER_RENDERERSERVICE_HPP
