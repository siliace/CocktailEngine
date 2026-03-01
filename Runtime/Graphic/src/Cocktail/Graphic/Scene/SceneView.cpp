#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/SceneView.hpp>
#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>

#include "Cocktail/Graphic/Scene/Light/Light.hpp"

namespace Ck
{
    class FrustumCullCollectAction : public SceneAction
    {
    public:

        explicit FrustumCullCollectAction(const Frustum<float>& frustum, Array<Renderable*>& renderables) :
            mFrustum(frustum),
            mRenderables(renderables)
        {
            mRenderables.Clear();
            mRenderables.Reserve(1024);
        }

        bool BeforeAccept(SceneNode* node) override
        {
            return node->IsVisible();
        }

        bool DoAccept(SceneNode* sceneNode) override
        {
            const Volume<float>& boundingVolume = sceneNode->GetBoundingVolume();
            if (boundingVolume.IsNull())
                return true;

            Box<float> obb;
            const Transformation& worldTransformation = sceneNode->GetWorldTransformation();
            if (!worldTransformation.IsIdentity())
            {
                for (std::size_t i = 0; i < boundingVolume.GetVertexCount(); i++)
                {
                    Vector3<float> vertex = boundingVolume.GetVertex(i);
                    Vector3<float> worldVertex = worldTransformation.Apply(vertex);
                    obb.Extend(worldVertex);
                }
            }
            else
            {
                obb.Extend(boundingVolume);
            }

            if (mFrustum.Intersect(obb) == Intersection::Outside)
                return false;

            mRenderables.Add(sceneNode);
            return true;
        }

        const Array<Renderable*>& GetRenderables() const
        {
            return mRenderables;
        }

    private:

        Frustum<float> mFrustum;
        Array<Renderable*>& mRenderables;
    };

    SceneView::SceneView(Scene* scene, Camera* camera) :
        mScene(scene),
        mCamera(camera)
    {
        /// Nothing
    }

    void SceneView::Update()
    {
        Frustum<float> frustum = mCamera->ComputeFrustum();

        mLights.Clear();
        for (const UniquePtr<Light>& light : mScene->GetLights())
        {
            if (NearlyEqual(light->GetIntensity(), 0.f))
                continue;

            if (light->FrustumCull(frustum) == Intersection::Outside)
                continue;

            mLights.Add(light.Get());
        }

        FrustumCullCollectAction action(frustum, mVisibleRenderables);
        mScene->PerformAction(action);
    }

    void SceneView::Bind(RecordDrawContext& drawContext) const
    {
        struct CameraInfo
        {
            Matrix4<float> ViewProjection;
            Vector3<float> ViewDirection;
        };

        CameraInfo cameraInfo;
        cameraInfo.ViewProjection = mCamera->ComputeProjectionViewMatrix().Transpose();
        cameraInfo.ViewDirection = mCamera->GetFront();
        drawContext.BindData(ShaderBindingDomain::Viewport, ViewportBindingSlots::CameraInfo, Renderer::BufferUsageFlagBits::Uniform, 0, sizeof(CameraInfo), &cameraInfo);
    }

    void SceneView::AddToQueue(RenderQueue& renderQueue)
    {
        for (Renderable* renderable : mVisibleRenderables)
            renderable->AddToQueue(renderQueue, mCamera);
    }

    const Array<Light*>& SceneView::GetLights() const
    {
        return mLights;
    }
}
