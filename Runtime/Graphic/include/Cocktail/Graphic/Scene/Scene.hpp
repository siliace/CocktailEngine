#ifndef COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENE_HPP

#include <Cocktail/Graphic/Mesh/Mesh.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/SceneGraph.hpp>
#include <Cocktail/Graphic/Spatial/TransformationGraph.hpp>

namespace Ck
{
    class Camera;
    class Light;

    class SceneAction
    {
    public:

        virtual ~SceneAction() = default;

        virtual bool BeforeAccept(SceneNode* sceneNode)
        {
            return true;
        }

        void Accept(SceneNode* sceneNode)
        {
            if (BeforeAccept(sceneNode))
            {
                if (DoAccept(sceneNode))
                {
                    for (SceneNode* child : sceneNode->GetChildren())
                        Accept(child);
                }

                AfterAccept(sceneNode);
            }
        }

        virtual void AfterAccept(SceneNode* sceneNode)
        {
        }

    protected:

        virtual bool DoAccept(SceneNode* sceneNode) = 0;
    };

    /**
     * \brief
     */
    class COCKTAIL_GRAPHIC_API Scene : public SharedFromThis<Scene>
    {
    public:

        /**
         * \brief
         * \param graphicEngine
         */
        explicit Scene(SharedPtr<GraphicEngine> graphicEngine);

        /**
         * \brief
         */
        ~Scene();

        /**
         * \brief
         * \param camera
         */
        void AddCamera(UniquePtr<Camera> camera);

        /**
         * \brief
         * \param camera
         */
        void RemoveCamera(const Camera* camera);

        /**
         * \brief
         * \param light
         */
        void AddLight(UniquePtr<Light> light);

        /**
         * \brief
         * \param light
         */
        void RemoveLight(const Light* light);

        /**
         * \brief
         * \return
         */
        SceneNode* CreateSceneNode();

        /**
         * \brief
         * \return
         */
        TransformationNode* CreateTransformationNode(const Transformation& transformation = Transformation::Identity());

        /**
         * \brief
         * \return
         */
        Signal<Camera*>& OnCameraAdded();

        /**
         * \brief
         * \return
         */
        Signal<Light*>& OnLightAdded();

        /**
         * \brief
         * \return
         */
        Signal<SceneNode*>& OnSceneNodeAdded();

        void PerformAction(SceneAction& action) const;

        const Array<UniquePtr<Camera>>& GetCameras() const;

        const Array<UniquePtr<Light>>& GetLights() const;

        /**
         * \brief
         * \return
         */
        SharedPtr<GraphicEngine> GetGraphicEngine() const;

    private:

        SharedPtr<GraphicEngine> mGraphicEngine;
        UniquePtr<TransformationGraph> mTransformationGraph;
        UniquePtr<SceneGraph> mSceneGraph;
        Array<UniquePtr<Camera>> mCameras;
        Array<UniquePtr<Light>> mLights;
        Signal<Camera*> mOnCameraAdded;
        Signal<Light*> mOnLightAdded;
        Signal<SceneNode*> mOnSceneNodeAdded;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
