#ifndef COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP

#include <CocktailEngine/Core/Signal/Observable.hpp>
#include <CocktailEngine/Core/Signal/Signal.hpp>

#include <CocktailEngine/Graphic/Rendering/Pipeline/RenderPipeline.hpp>
#include <CocktailEngine/Graphic/Rendering/Queue/RecordDrawContext.hpp>
#include <CocktailEngine/Graphic/Rendering/SceneRenderSettings.hpp>
#include <CocktailEngine/Graphic/Rendering/Viewport.hpp>

#include <CocktailEngine/Renderer/Context/RenderContext.hpp>
#include <CocktailEngine/Renderer/Context/RenderSurface.hpp>

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
    class COCKTAILENGINE_GRAPHIC_API SceneViewer : public Observable
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
         * \brief Get the mutable per-frame scene render settings
         *
         * \return The scene render settings
         */
        SceneRenderSettings& GetRenderSettings();

        /**
         * \brief Get the per-frame scene render settings
         *
         * \return The scene render settings
         */
        const SceneRenderSettings& GetRenderSettings() const;

        /**
         * \brief Replace the render pipeline used to draw the scene
         *
         * Allows injecting a custom rendering strategy (deferred, clustered, etc.)
         * in place of the default forward pipeline.
         *
         * \param pipeline The pipeline to use
         */
        void SetRenderPipeline(UniquePtr<RenderPipeline> pipeline);

        /**
         * \brief Get the render pipeline currently in use
         *
         * \return The render pipeline
         */
        RenderPipeline* GetRenderPipeline() const;

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
        SceneRenderSettings mRenderSettings;
        UniquePtr<RenderPipeline> mRenderPipeline;
        Signal<Renderer::RenderContext&, Renderer::Framebuffer&> mOnRendered;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_SCENEVIEWER_HPP
