#include <cmath>

#include <CocktailEngine/Graphic/Rendering/Lighting/DefaultLightCollector.hpp>
#include <CocktailEngine/Graphic/Rendering/Queue/RecordDrawContext.hpp>
#include <CocktailEngine/Graphic/Scene/Light/DirectionalLight.hpp>
#include <CocktailEngine/Graphic/Scene/Light/PointLight.hpp>
#include <CocktailEngine/Graphic/Scene/Light/SpotLight.hpp>
#include <CocktailEngine/Graphic/Scene/SceneView.hpp>

namespace Ck
{
    void DefaultLightCollector::Collect(SceneView& sceneView, RecordDrawContext& drawContext)
    {
        const Array<Light*>& lights = sceneView.GetLights();

        struct LightInstance
        {
            LinearColor Color;
            Vector3<float> Position;
            float Intensity;
            Vector3<float> Direction;
            unsigned int Type;
            Vector2<float> CutOff;
            float Constants;
            float Linear;
            float Quadratic;
            float __Padding[3];
        };

        unsigned int lightCount = lights.GetSize();
        LightInstance* lightsInfo = COCKTAIL_STACK_ALLOC(LightInstance, lightCount);
        for (unsigned int i = 0; i < lightCount; i++)
        {
            LightInstance& lightInstance = lightsInfo[i];
            lightInstance.Type = static_cast<unsigned int>(lights[i]->GetType());
            lightInstance.Color = lights[i]->GetColor();
            lightInstance.Intensity = lights[i]->GetIntensity() / 100.f;

            if (lights[i]->GetType() != Light::Type::Directional)
            {
                if (lights[i]->GetType() == Light::Type::Spot)
                {
                    SpotLight* spotLight = static_cast<SpotLight*>(lights[i]);
                    lightInstance.Position = spotLight->GetWorldTransformation().GetTranslation();
                    lightInstance.Direction = -spotLight->GetFront();
                    lightInstance.Constants = spotLight->GetConstantAttenuationFactor();
                    lightInstance.Linear = spotLight->GetLinearAttenuationFactor();
                    lightInstance.Quadratic = spotLight->GetQuadraticAttenuationFactor();
                    lightInstance.CutOff.X() = std::cos(spotLight->GetInnerCutoff());
                    lightInstance.CutOff.Y() = std::cos(spotLight->GetOuterCutoff());
                }
                else if (lights[i]->GetType() == Light::Type::Point)
                {
                    PointLight* pointLight = static_cast<PointLight*>(lights[i]);
                    lightInstance.Position = pointLight->GetWorldTransformation().GetTranslation();
                    lightInstance.Constants = pointLight->GetConstantAttenuationFactor();
                    lightInstance.Linear = pointLight->GetLinearAttenuationFactor();
                    lightInstance.Quadratic = pointLight->GetQuadraticAttenuationFactor();
                }
            }
            else
            {
                DirectionalLight* directionalLight = static_cast<DirectionalLight*>(lights[i]);
                lightInstance.Direction = directionalLight->GetDirection();
            }
        }

        drawContext.BindData(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, Renderer::BufferUsageFlagBits::Storage, 0,
                             lightCount * sizeof(LightInstance), lightsInfo);
    }
}
