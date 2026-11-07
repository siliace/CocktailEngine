#ifndef COCKTAIL_GRAPHIC_RENDERING_PASS_FORWARDRENDERPASS_HPP
#define COCKTAIL_GRAPHIC_RENDERING_PASS_FORWARDRENDERPASS_HPP

#include <CocktailEngine/Graphic/Rendering/Pass/RenderPass.hpp>
#include <CocktailEngine/Graphic/Rendering/Queue/RenderQueue.hpp>

namespace Ck
{
    class MaterialProgramManager;

    /**
     * \brief Forward rendering pass backed by a single RenderQueue
     *
     * A ForwardRenderPass owns one RenderQueue configured for a specific blending
     * mode. During Prepare() it collects the visible renderables of the view into
     * its queue, and during Execute() it sorts and flushes that queue to the
     * command list.
     *
     * The opaque and transparent passes of the default pipeline are simply two
     * instances of this class configured with different blending modes.
     */
    class COCKTAILENGINE_GRAPHIC_API ForwardRenderPass : public RenderPass
    {
    public:

        /**
         * \brief Constructor
         *
         * \param name Human readable name of the pass
         * \param materialProgramManager Manager used to resolve GPU programs
         * \param blendingMode Blending mode applied to the underlying queue
         */
        ForwardRenderPass(const char* name, SharedPtr<MaterialProgramManager> materialProgramManager, RenderQueue::BlendingMode blendingMode);

        /**
         * \copydoc RenderPass::Prepare
         */
        void Prepare(SceneView& sceneView, RecordDrawContext& drawContext) override;

        /**
         * \copydoc RenderPass::Execute
         */
        void Execute(Renderer::CommandList& commandList, RecordDrawContext& drawContext) override;

        /**
         * \copydoc RenderPass::GetName
         */
        const char* GetName() const override;

        /**
         * \brief Get the queue backing this pass
         *
         * \return The render queue
         */
        RenderQueue& GetQueue();

    private:

        const char* mName;
        RenderQueue mQueue;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_PASS_FORWARDRENDERPASS_HPP
