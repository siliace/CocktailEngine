#ifndef COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP

#include <set>

#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

#include <Cocktail/Graphic/Rendering/Viewport.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>

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
	class COCKTAIL_GRAPHIC_API SceneViewer : public Extends<SceneViewer, Object>, public Observable
	{
	public:

		/**
		 * \brief
		 * \param viewport
		 * \param index
		 */
		void AttachViewport(Ref<Viewport> viewport, unsigned int index = 0);

		/**
		 * \brief
		 * \param viewport
		 */
		void DetachViewport(const Ref<Viewport>& viewport);

		/**
		 * \brief 
		 */
		void Render();

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Scene> GetScene() const;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Renderer::FrameContext&, Renderer::Framebuffer&>& OnRendered();

	protected:

		/**
		 * \brief
		 * \param scene
		 */
		explicit SceneViewer(Ref<Scene> scene);

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
			Ref<Viewport> Viewport;

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

		Ref<Scene> mScene;
		std::set<ViewportEntry, ViewportComparator> mViewports;
		Ref<RenderQueue> mRenderQueue;
		Signal<Renderer::FrameContext&, Renderer::Framebuffer&> mOnRendered;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
