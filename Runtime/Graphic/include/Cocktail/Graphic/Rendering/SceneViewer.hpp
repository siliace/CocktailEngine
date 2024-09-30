#ifndef COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP

#include <set>

#include <Cocktail/Graphic/Rendering/Viewport.hpp>
#include <Cocktail/Graphic/Rendering/Renderable.hpp>
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
	class COCKTAIL_GRAPHIC_API SceneViewer : public Extends<SceneViewer, Object>, public Observable
	{
	public:

		/**
		 * \brief 
		 */
		void Render() const;

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
		 * \return 
		 */
		Ref<Scene> GetScene() const;

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
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
