#ifndef COCKTAIL_GRAPHIC_RENDERING_WINDOWSCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_WINDOWSCENEVIEWER_HPP

#include <Cocktail/Graphic/Rendering/SceneViewer.hpp>

namespace Ck
{
	class Window;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API WindowSceneViewer : public Extends<WindowSceneViewer, SceneViewer>
	{
	public:

		/**
		 * \brief 
		 * \param scene 
		 * \param window 
		 * \param parameters 
		 * \param vSync 
		 */
		WindowSceneViewer(Ref<Scene> scene, Ref<Window> window, SceneViewerParameters parameters, bool vSync);

	protected:

		/**
		 * \brief 
		 * \param frameContext 
		 * \return 
		 */
		Renderer::Framebuffer* AcquireNextFramebuffer(Renderer::FrameContext& frameContext) const override;

	private:

		Ref<Renderer::RenderSurface> mRenderSurface;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_WINDOWSCENEVIEWER_HPP
