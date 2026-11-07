#ifndef COCKTAIL_GRAPHIC_RENDERING_LIGHTING_LIGHTCOLLECTOR_HPP
#define COCKTAIL_GRAPHIC_RENDERING_LIGHTING_LIGHTCOLLECTOR_HPP

#include <CocktailEngine/Graphic/Export.hpp>

namespace Ck
{
    class RecordDrawContext;
    class SceneView;

    /**
     * \brief Collects the lights of a SceneView and binds them for rendering
     *
     * A LightCollector is responsible for turning the high-level Light objects
     * visible in a SceneView into the GPU friendly representation expected by the
     * shaders, then binding that data into a RecordDrawContext.
     *
     * Extracting this responsibility out of SceneViewer allows different lighting
     * strategies (clustered, tiled, shadow-casting, etc.) to be plugged in without
     * modifying the viewer itself.
     */
    class COCKTAILENGINE_GRAPHIC_API LightCollector
    {
    public:

        /**
         * \brief Virtual destructor
         */
        virtual ~LightCollector() = default;

        /**
         * \brief Collect and bind the lights of a SceneView
         *
         * \param sceneView The scene view whose lights must be collected
         * \param drawContext The draw context receiving the packed light data
         */
        virtual void Collect(SceneView& sceneView, RecordDrawContext& drawContext) = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_LIGHTING_LIGHTCOLLECTOR_HPP
