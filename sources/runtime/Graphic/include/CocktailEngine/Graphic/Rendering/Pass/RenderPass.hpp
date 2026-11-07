#ifndef COCKTAIL_GRAPHIC_RENDERING_PASS_RENDERPASS_HPP
#define COCKTAIL_GRAPHIC_RENDERING_PASS_RENDERPASS_HPP

#include <CocktailEngine/Graphic/Export.hpp>

#include <CocktailEngine/Renderer/Command/CommandList.hpp>

namespace Ck
{
    class RecordDrawContext;
    class SceneView;

    /**
     * \brief A single stage of a rendering pipeline
     *
     * A RenderPass encapsulates everything needed to render one logical step of a
     * frame (an opaque pass, a transparent pass, a depth pre-pass, a shadow pass,
     * a post-process pass, etc.).
     *
     * The rendering of a pass is split in two phases so a pipeline can first
     * prepare every pass, then execute them in order:
     *  - Prepare(): gather the renderables / state the pass needs for the frame.
     *  - Execute(): record the actual draw commands into the command list.
     *
     * Implementing a new rendering step is now a matter of adding a RenderPass
     * subclass instead of modifying SceneViewer.
     */
    class COCKTAILENGINE_GRAPHIC_API RenderPass
    {
    public:

        /**
         * \brief Virtual destructor
         */
        virtual ~RenderPass() = default;

        /**
         * \brief Gather the data required by the pass for the current view
         *
         * Called once per viewport before Execute(). Implementations typically
         * collect the visible renderables of \p sceneView into an internal queue.
         *
         * \param sceneView The scene view being rendered
         * \param drawContext The draw context shared across passes
         */
        virtual void Prepare(SceneView& sceneView, RecordDrawContext& drawContext) = 0;

        /**
         * \brief Record the draw commands of the pass
         *
         * \param commandList The command list receiving the draw commands
         * \param drawContext The draw context shared across passes
         */
        virtual void Execute(Renderer::CommandList& commandList, RecordDrawContext& drawContext) = 0;

        /**
         * \brief Get a human readable name identifying the pass
         *
         * \return The pass name
         */
        virtual const char* GetName() const = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_PASS_RENDERPASS_HPP
