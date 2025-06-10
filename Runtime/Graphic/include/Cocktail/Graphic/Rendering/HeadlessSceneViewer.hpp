#ifndef COCKTAIL_GRAPHIC_RENDERING_HEADLESSSCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_HEADLESSSCENEVIEWER_HPP

#include <Cocktail/Graphic/Rendering/SceneViewer.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API HeadlessSceneViewer : public SceneViewer
	{
	public:

		HeadlessSceneViewer(std::shared_ptr<Scene> scene, Extent2D<unsigned int> size, SceneViewerParameters parameters);

	protected:

		Renderer::Framebuffer* AcquireNextFramebuffer(Renderer::RenderContext& renderContext) const override;

	private:

		mutable unsigned int mCurrentFramebuffer;
		std::vector<std::shared_ptr<Renderer::Framebuffer>> mFramebuffers;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_HEADLESSSCENEVIEWER_HPP
