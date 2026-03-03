#include <imgui.h>

#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>

#include <Cocktail/Sponza/ImUi/ImUi.hpp>
#include <Cocktail/Sponza/ImUi/Panel/PanelManager.hpp>
#include <Cocktail/Sponza/SceneMenuModule.hpp>

namespace Ck
{
    class CameraPanel : public ImUi::Panel
    {
    public:

        explicit CameraPanel(Scene* scene) :
            mScene(scene),
            mCurrentCamera(nullptr)
        {
            /// Nothing
        }

        void Render() override
        {
            if (ImGui::Begin("Cameras", &mOpen))
            {
                if (ImGui::BeginCombo("Select light", mCurrentCamera ? CK_TEXT_TO_IMGUI(mCurrentCamera->GetName()) : nullptr))
                {
                    for (const UniquePtr<Camera>& camera : mScene->GetCameras())
                    {
                        const bool selected = camera.Get() == mCurrentCamera;
                        if (ImGui::Selectable(CK_TEXT_TO_IMGUI(camera->GetName()), selected))
                            mCurrentCamera = camera.Get();

                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                ImGui::Separator();
            }

            ImGui::End();
        }

        const AnsiChar* GetName() const override
        {
            return "Camera";
        }

    private:

        Scene* mScene;
        Camera* mCurrentCamera;
    };

    class LightPanel : public ImUi::Panel
    {
    public:

        explicit LightPanel(Scene* scene) :
            mScene(scene),
            mCurrentLight(nullptr)
        {
            /// Nothing
        }

        void Render() override
        {
            if (ImGui::Begin("Lights", &mOpen))
            {
                if (ImGui::BeginCombo("Select light", mCurrentLight ? CK_TEXT_TO_IMGUI(mCurrentLight->GetName()) : nullptr))
                {
                    for (const UniquePtr<Light>& light : mScene->GetLights())
                    {
                        const bool selected = light.Get() == mCurrentLight;
                        if (ImGui::Selectable(CK_TEXT_TO_IMGUI(light->GetName()), selected))
                            mCurrentLight = light.Get();

                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                ImGui::Separator();

                if (mCurrentLight)
                {
                    LinearColor lightColor = mCurrentLight->GetColor();
                    float color[3] = { lightColor.R, lightColor.G, lightColor.B };
                    if (ImGui::ColorPicker3("ColorPick", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float))
                        mCurrentLight->SetColor(LinearColor(color[0], color[1], color[2]));

                    ImGui::SameLine();

                    float lightIntensity = mCurrentLight->GetIntensity();
                    if (ImGui::DragFloat("Intensity", &lightIntensity, 0.0f, 100.0f))
                        mCurrentLight->SetIntensity(lightIntensity);

                    ImGui::Separator();

                    if (mCurrentLight->GetType() == Light::Type::Directional)
                    {
                        DirectionalLight* directionalLight = static_cast<DirectionalLight*>(mCurrentLight);

                        Vector3<float> lightDirection = directionalLight->GetDirection();
                        ImGui::PushID("Light");

                        bool changed = false;
                        changed |= ImGui::DragFloat("X", &lightDirection.X(), 0.01f, -1.0f, 1.0f, "%.3f");
                        changed |= ImGui::DragFloat("Y", &lightDirection.Y(), 0.01f, -1.0f, 1.0f, "%.3f");
                        changed |= ImGui::DragFloat("Z", &lightDirection.Z(), 0.01f, -1.0f, 1.0f, "%.3f");
                        if (changed)
                            directionalLight->SetDirection(lightDirection.Normalized());

                        ImGui::PopID();
                    }
                }
            }
            ImGui::End();
        }

        const AnsiChar* GetName() const override
        {
            return "Light";
        }

    private:

        Scene* mScene;
        Light* mCurrentLight;
    };

    SceneMenuModule::SceneMenuModule(Scene* scene) :
        mScene(scene)
    {
        /// Nothing
    }

    void SceneMenuModule::Attach()
    {
        MenuModule::Attach();

        CreatePanel([&]() {
            return GetPanelManager()->CreatePanel<CameraPanel>(mScene);
        });

        CreatePanel([&]() {
            return GetPanelManager()->CreatePanel<LightPanel>(mScene);
        });
    }

    const AnsiChar* SceneMenuModule::GetName() const
    {
        return "Scene";
    }
}
