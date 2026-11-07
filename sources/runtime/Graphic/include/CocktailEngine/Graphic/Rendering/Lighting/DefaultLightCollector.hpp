#ifndef COCKTAIL_GRAPHIC_RENDERING_LIGHTING_DEFAULTLIGHTCOLLECTOR_HPP
#define COCKTAIL_GRAPHIC_RENDERING_LIGHTING_DEFAULTLIGHTCOLLECTOR_HPP

#include <CocktailEngine/Graphic/Rendering/Lighting/LightCollector.hpp>

namespace Ck
{
    /**
     * \brief Default forward-rendering light collector
     *
     * Packs every visible light of a SceneView into a single storage buffer and
     * binds it to the Viewport binding domain. This reproduces the behaviour that
     * used to be inlined in SceneViewer::Render().
     */
    class COCKTAILENGINE_GRAPHIC_API DefaultLightCollector : public LightCollector
    {
    public:

        /**
         * \copydoc LightCollector::Collect
         */
        void Collect(SceneView& sceneView, RecordDrawContext& drawContext) override;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_LIGHTING_DEFAULTLIGHTCOLLECTOR_HPP
