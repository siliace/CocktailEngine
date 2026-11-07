#ifndef COCKTAIL_GRAPHIC_RENDERING_PIPELINE_RENDERPIPELINE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_PIPELINE_RENDERPIPELINE_HPP

#include <CocktailEngine/Core/Array.hpp>

#include <CocktailEngine/Graphic/Export.hpp>
#include <CocktailEngine/Graphic/Rendering/SceneRenderSettings.hpp>

#include <CocktailEngine/Renderer/Command/CommandList.hpp>
#include <CocktailEngine/Renderer/Framebuffer/Framebuffer.hpp>

namespace Ck
{
    class RecordDrawContext;
    class Viewport;

    /**
     * \brief Describes and executes the sequence of passes that render a frame
     *
     * A RenderPipeline encapsulates the full rendering strategy: which passes run,
     * in which order, and how global/view data is bound. SceneViewer delegates the
     * frame rendering to a RenderPipeline instead of hard-coding opaque/transparent
     * queues, so alternative strategies (deferred, clustered, custom) can be
     * injected without touching the viewer.
     */
    class COCKTAILENGINE_GRAPHIC_API RenderPipeline
    {
    public:

        /**
         * \brief Virtual destructor
         */
        virtual ~RenderPipeline() = default;

        /**
         * \brief Render every viewport of a scene for the current frame
         *
         * \param viewports The viewports to render, in order
         * \param commandList The command list receiving the draw commands
         * \param framebuffer The target framebuffer
         * \param drawContext The draw context shared across passes
         * \param settings Global per-frame scene settings
         */
        virtual void Render(const Array<UniquePtr<Viewport>>& viewports, Renderer::CommandList& commandList, Renderer::Framebuffer& framebuffer,
                            RecordDrawContext& drawContext, const SceneRenderSettings& settings) = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_PIPELINE_RENDERPIPELINE_HPP
