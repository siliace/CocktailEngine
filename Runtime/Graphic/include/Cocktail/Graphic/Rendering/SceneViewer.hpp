#ifndef COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP

#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

#include <Cocktail/Graphic/Rendering/Viewport.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>

#include <Cocktail/Renderer/Context/RenderContext.hpp>
#include <Cocktail/Renderer/Context/RenderSurface.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief 
	 */
	struct SceneViewerParameters
	{
		/**
		 * \brief 
		 */
		PixelFormat DepthStencilFormat = PixelFormat::Undefined();

		/**
		 * \brief 
		 */
		Renderer::RasterizationSamples Samples = Renderer::RasterizationSamples::e1;

		/**
		 * \brief 
		 */
		unsigned int FrameCount = 2;
	};

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API SceneViewer : public Observable
	{
	public:

		/**
		 * \brief
		 * \param viewport
		 */
		void AttachViewport(UniquePtr<Viewport> viewport);

        /**
         * \brief
         *
         * \param size
         */
	    void Resize(const Extent2D<unsigned int>& size);

		/**
		 * \brief 
		 */
		void Render();

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<Scene> GetScene() const;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Renderer::RenderContext&, Renderer::Framebuffer&>& OnRendered();

	protected:

		/**
		 * \brief
		 * \param scene
		 */
		explicit SceneViewer(SharedPtr<Scene> scene);

		/**
		 * \brief 
		 * \param renderContext 
		 * \return 
		 */
		virtual Renderer::Framebuffer* AcquireNextFramebuffer(Renderer::RenderContext& renderContext) const = 0;

	private:

		SharedPtr<Scene> mScene;
	    RecordDrawContext mDrawContext;
		Array<UniquePtr<Viewport>> mViewports;
		UniquePtr<RenderQueue> mOpaqueRenderQueue;
		UniquePtr<RenderQueue> mBlendingRenderQueue;
		Signal<Renderer::RenderContext&, Renderer::Framebuffer&> mOnRendered;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
