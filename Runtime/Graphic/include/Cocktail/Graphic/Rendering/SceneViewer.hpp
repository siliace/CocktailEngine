#ifndef COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP

#include <set>

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
		 * \param index
		 */
		void AttachViewport(std::shared_ptr<Viewport> viewport, unsigned int index = 0);

		/**
		 * \brief
		 * \param viewport
		 */
		void DetachViewport(const std::shared_ptr<Viewport>& viewport);

		/**
		 * \brief 
		 */
		void Render();

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Scene> GetScene() const;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Renderer::RenderContext&, Renderer::FrameContext&, Renderer::Framebuffer&>& OnRendered();

	protected:

		/**
		 * \brief
		 * \param scene
		 */
		explicit SceneViewer(std::shared_ptr<Scene> scene);

		/**
		 * \brief 
		 * \param frameContext 
		 * \return 
		 */
		virtual Renderer::Framebuffer* AcquireNextFramebuffer(Renderer::FrameContext& frameContext) const = 0;

	private:

		/**
		 * \brief 
		 */
		struct ViewportEntry
		{
			/**
			 * \brief 
			 */
			std::shared_ptr<Viewport> Viewport;

			/**
			 * \brief 
			 */
			unsigned int Index = 0;
		};

		/**
		 * \brief 
		 */
		struct ViewportComparator
		{
			/**
			 * \brief 
			 * \param lhs 
			 * \param rhs 
			 * \return 
			 */
			int operator()(const ViewportEntry& lhs, const ViewportEntry& rhs) const noexcept;
		};

		std::shared_ptr<Scene> mScene;
		std::set<ViewportEntry, ViewportComparator> mViewports;
		std::unique_ptr<RenderQueue> mOpaqueRenderQueue;
		std::unique_ptr<RenderQueue> mBlendingRenderQueue;
		Signal<Renderer::RenderContext&, Renderer::FrameContext&, Renderer::Framebuffer&> mOnRendered;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
