#ifndef COCKTAIL_GRAPHIC_RENDERING_PIPELINE_FORWARDRENDERPIPELINE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_PIPELINE_FORWARDRENDERPIPELINE_HPP

#include <CocktailEngine/Graphic/Rendering/Lighting/LightCollector.hpp>
#include <CocktailEngine/Graphic/Rendering/Pass/RenderPass.hpp>
#include <CocktailEngine/Graphic/Rendering/Pipeline/RenderPipeline.hpp>

namespace Ck
{
    class MaterialProgramManager;

    /**
     * \brief Default forward rendering pipeline
     *
     * Reproduces the historical SceneViewer behaviour: for every viewport it binds
     * the view data, collects the lights, then runs an opaque pass followed by a
     * transparent pass.
     *
     * The pipeline is built from a list of RenderPass instances and a
     * LightCollector, both of which can be customised. Additional passes can be
     * appended through AddPass() to extend the pipeline without subclassing.
     */
    class COCKTAILENGINE_GRAPHIC_API ForwardRenderPipeline : public RenderPipeline
    {
    public:

        /**
         * \brief Build the default forward pipeline (opaque + transparent passes)
         *
         * \param materialProgramManager Manager used to resolve GPU programs
         */
        explicit ForwardRenderPipeline(SharedPtr<MaterialProgramManager> materialProgramManager);

        /**
         * \brief Append a custom pass at the end of the pipeline
         *
         * \param pass The pass to add
         */
        void AddPass(UniquePtr<RenderPass> pass);

        /**
         * \brief Replace the light collector used by the pipeline
         *
         * \param lightCollector The new light collector
         */
        void SetLightCollector(UniquePtr<LightCollector> lightCollector);

        /**
         * \copydoc RenderPipeline::Render
         */
        void Render(const Array<UniquePtr<Viewport>>& viewports, Renderer::CommandList& commandList, Renderer::Framebuffer& framebuffer,
                    RecordDrawContext& drawContext, const SceneRenderSettings& settings) override;

    private:

        UniquePtr<LightCollector> mLightCollector;
        Array<UniquePtr<RenderPass>> mPasses;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_PIPELINE_FORWARDRENDERPIPELINE_HPP
