#ifndef COCKTAIL_GRAPHIC_RENDERING_WINDOWSCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_WINDOWSCENEVIEWER_HPP

#include <Cocktail/Graphic/Rendering/SceneViewer.hpp>

namespace Ck
{
	class Window;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API WindowSceneViewer : public SceneViewer
	{
	public:

		/**
		 * \brief 
		 * \param scene 
		 * \param window 
		 * \param parameters 
		 * \param vSync 
		 */
		WindowSceneViewer(std::shared_ptr<Scene> scene, std::shared_ptr<Window> window, SceneViewerParameters parameters, bool vSync);

	protected:

		/**
		 * \brief 
		 * \param renderContext 
		 * \return 
		 */
		Renderer::Framebuffer* AcquireNextFramebuffer(Renderer::RenderContext& renderContext) const override;

	private:

		std::shared_ptr<Renderer::RenderSurface> mRenderSurface;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_WINDOWSCENEVIEWER_HPP
