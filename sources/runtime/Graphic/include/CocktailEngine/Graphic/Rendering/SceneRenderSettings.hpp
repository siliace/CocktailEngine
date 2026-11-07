#ifndef COCKTAIL_GRAPHIC_RENDERING_SCENERENDERSETTINGS_HPP
#define COCKTAIL_GRAPHIC_RENDERING_SCENERENDERSETTINGS_HPP

#include <CocktailEngine/Graphic/Export.hpp>

namespace Ck
{
    /**
     * \brief Global, per-frame rendering settings shared across the whole scene
     *
     * These values are bound in the Scene shader binding domain at the beginning
     * of every frame. Keeping them in a dedicated structure removes the hard-coded
     * magic numbers that used to live inside SceneViewer::Render() and makes the
     * behaviour configurable per viewer.
     */
    struct SceneRenderSettings
    {
        /**
         * \brief Factor applied to the ambient lighting term
         *
         * Controls how much constant ambient light is added to every fragment.
         */
        float AmbientFactor = 0.1f;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_SCENERENDERSETTINGS_HPP
