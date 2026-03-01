#ifndef COCKTAIL_GRAPHIC_SCENE_SCENEVIEW_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENEVIEW_HPP

#include <Cocktail/Core/Array.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
    class Camera;
    class Light;
    class Renderable;
    class RecordDrawContext;
    class RenderQueue;
    class Scene;

    /**
     * \brief Class representing a view form a camera of a scene
     */
    class COCKTAIL_GRAPHIC_API SceneView
    {
    public:

        /**
         * \brief Constructor
         *
         * Creates a new instance of SceneView.
         *
         * \param scene The scene to watch
         * \param camera The camera viewing the scene
         *
         * \pre \p camera should have been added in \p scene
         */
        SceneView(Scene* scene, Camera* camera);

        /**
         * \brief Updates the SceneView
         *
         * This will recompute the list of visible Renderable objects
         */
        void Update();

        /**
         * \brief Bind SceneView relative data in a RecordDrawContext
         *
         * \param drawContext The RecordDrawContext
         */
        void Bind(RecordDrawContext& drawContext) const;

        /**
         * \brief Add to every Renderable visible to a \p renderQueue
         *
         * \param renderQueue The RenderQueue where renderables object should be added
         */
        void AddToQueue(RenderQueue& renderQueue);

        /**
         * \brief Get the list of Light having an impact on Renderable objects visibles in this SceneView
         *
         * \return The list of lights
         */
        const Array<Light*>& GetLights() const;

    private:

        Scene* mScene;
        Camera* mCamera;
        Array<Light*> mLights;
        Array<Renderable*> mVisibleRenderables;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENEVIEW_HPP
