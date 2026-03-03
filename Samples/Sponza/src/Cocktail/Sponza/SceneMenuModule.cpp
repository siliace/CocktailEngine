#include <imgui.h>

#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>
#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

#include <Cocktail/Sponza/ImUi/ImUi.hpp>
#include <Cocktail/Sponza/ImUi/Panel/PanelManager.hpp>
#include <Cocktail/Sponza/SceneMenuModule.hpp>

#include "Cocktail/Graphic/Scene/Camera/OrhtographicCamera.hpp"
#include "Cocktail/Graphic/Scene/Light/PointLight.hpp"
#include "Cocktail/Graphic/Scene/Light/SpotLight.hpp"

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
                if (ImGui::BeginCombo("Select camera", mCurrentCamera ? CK_TEXT_TO_IMGUI(mCurrentCamera->GetName()) : nullptr))
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

                if (mCurrentCamera)
                {
                    if (mCurrentCamera->GetType() == Camera::Type::Perspective)
                    {
                        PerspectiveCamera* perspectiveCamera = static_cast<PerspectiveCamera*>(mCurrentCamera);

                        float fov = perspectiveCamera->GetFov().AsDegree();
                        if (ImGui::InputFloat("Fov", &fov, 1.f, 5.f, "%.1f"))
                            perspectiveCamera->SetFov(Angle<float>::Degree(std::clamp(fov, 20.f, 80.f)));

                        float aspectRatio = perspectiveCamera->GetAspectRatio();
                        if (ImGui::InputFloat("Ratio", &aspectRatio, 0.1f, 0.5f, "%.3f"))
                            perspectiveCamera->SetAspectRatio(aspectRatio);
                    }
                    else
                    {
                        OrthographicCamera* orthographicCamera = static_cast<OrthographicCamera*>(mCurrentCamera);

                        Rectangle<float> area = orthographicCamera->GetArea();
                        float left = area.Position.X();
                        float right = left + area.Extent.Width;
                        float bottom = area.Position.Y();
                        float top = bottom + area.Extent.Height;

                        float horizontalMagnification = (right - left) * 0.5f;
                        float verticalMagnification = (top - bottom) * 0.5f;

                        bool recompute = false;
                        if (ImGui::InputFloat("Horizontal magnification", &horizontalMagnification, 0.1f, 0.5f))
                            recompute = true;

                        if (ImGui::InputFloat("Vertical magnification", &verticalMagnification, 0.1f, 0.5f))
                            recompute = true;

                        if (recompute)
                        {
                            Vector2<float> position;
                            position.X() = -horizontalMagnification;
                            position.Y() = -verticalMagnification;

                            Extent2D<float> extent;
                            extent.Width = 2.f * horizontalMagnification;
                            extent.Height = 2.f * verticalMagnification;

                            orthographicCamera->SetArea(Rectangle(position, extent));
                        }
                    }
                }
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
            mCreate(false)
        {
            /// Nothing
        }

        void Render() override
        {
            if (ImGui::Begin("Lights", &mOpen))
            {
                bool resetState = false;
                if (!mCreate)
                {
                    if (ImGui::BeginCombo("Select light", CK_TEXT_TO_IMGUI(mState.Name)))
                    {
                        for (const UniquePtr<Light>& light : mScene->GetLights())
                        {
                            const bool selected = light.Get() == mCurrentLight;
                            if (ImGui::Selectable(CK_TEXT_TO_IMGUI(light->GetName()), selected))
                                resetState = CheckedAssign(mCurrentLight, light.Get());

                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Create", ImVec2(150.f, 25.f)))
                        mCreate = resetState = true;

                    ImGui::Separator();
                }

                if (resetState)
                {
                    if (!mCreate && mCurrentLight)
                    {
                        mState.Type = Optional<Light::Type>::Of(mCurrentLight->GetType());
                        mState.Name = mCurrentLight->GetName();
                        mState.Color = mCurrentLight->GetColor();
                        mState.Intensity = mCurrentLight->GetIntensity();

                        if (mCurrentLight->GetType() == Light::Type::Directional)
                        {
                            mState.Directional.Direction = static_cast<DirectionalLight*>(mCurrentLight)->GetDirection();
                        }
                        else if (mCurrentLight->GetType() == Light::Type::Point)
                        {
                            PointLight* pointLight = static_cast<PointLight*>(mCurrentLight);

                            mState.Point.Range = pointLight->GetRange();
                            mState.Point.Position = pointLight->GetPosition();
                        }
                        else if (mCurrentLight->GetType() == Light::Type::Spot)
                        {
                            SpotLight* spotLight = static_cast<SpotLight*>(mCurrentLight);

                            mState.Spot.Range = spotLight->GetRange();
                            mState.Spot.Position = spotLight->GetPosition();
                            mState.Spot.InnerCutoff = spotLight->GetInnerCutoff();
                            mState.Spot.OuterCutoff = spotLight->GetOuterCutoff();
                        }
                    }
                    else
                    {
                        mState = {};
                        mCurrentLight = nullptr;
                    }
                }

                if (mCreate || mCurrentLight)
                {
                    if (mCreate)
                    {
                        const AnsiChar* lightTypeName = mState.Type.Map([](Light::Type lightType) {
                            return Enum<Light::Type>::ToString(lightType);
                        }).GetOr(nullptr);

                        if (ImGui::BeginCombo("Type", lightTypeName))
                        {
                            for (Light::Type lightType : Enum<Light::Type>::Values)
                            {
                                const bool selected = !mState.Type.IsEmpty() ? mState.Type.Get() == lightType : false;
                                if (ImGui::Selectable(Enum<Light::Type>::ToString(lightType), selected))
                                    mState.Type = Optional<Light::Type>::Of(lightType);

                                if (selected)
                                    ImGui::SetItemDefaultFocus();
                            }

                            ImGui::EndCombo();
                        }

                        auto resizeCallback = [](ImGuiInputTextCallbackData* data) {
                            if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                            {
                                Utf8String* string = (Utf8String*)data->UserData;
                                string->Resize(data->BufTextLen);
                                data->Buf = (char*)string->GetData();
                            }

                            return 0;
                        };

                        Utf8String name = Utf8String::Convert(mState.Name);
                        name.EnsureSize(16);
                        if (ImGui::InputText("Name", reinterpret_cast<char*>(name.GetData()), name.GetLength(), ImGuiInputTextFlags_CallbackResize, resizeCallback, &name))
                        {
                            mState.Name = String::Convert(name);
                        }
                    }

                    ImGui::Separator();

                    LinearColor lightColor = mState.Color;
                    float color[3] = { lightColor.R, lightColor.G, lightColor.B };
                    if (ImGui::ColorPicker3("ColorPick", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float))
                    {
                        mState.Color = LinearColor(color[0], color[1], color[2]);
                        if (mCurrentLight)
                            mCurrentLight->SetColor(mState.Color);
                    }

                    ImGui::Separator();

                    float intensity = mState.Intensity;
                    if (ImGui::DragFloat("Intensity", &intensity, 1.f, 0.f, 100.f, "%.0f"))
                    {
                        mState.Intensity = std::clamp(intensity, 0.f, 100.f);
                        if (mCurrentLight)
                            mCurrentLight->SetIntensity(intensity);
                    }

                    ImGui::Separator();

                    if (!mState.Type.IsEmpty())
                    {
                        if (mState.Type.Get() == Light::Type::Directional)
                        {
                            bool changed = false;
                            Vector3<float> direction = mState.Directional.Direction;
                            changed |= ImGui::DragFloat("X", &direction.X(), 0.01f, -1.0f, 1.0f, "%.3f");
                            changed |= ImGui::DragFloat("Y", &direction.Y(), 0.01f, -1.0f, 1.0f, "%.3f");
                            changed |= ImGui::DragFloat("Z", &direction.Z(), 0.01f, -1.0f, 1.0f, "%.3f");
                            if (changed)
                            {
                                mState.Directional.Direction = direction.Normalized();
                                if (mCurrentLight)
                                    static_cast<DirectionalLight*>(mCurrentLight)->SetDirection(mState.Directional.Direction);
                            }
                        }
                        else if (mState.Type.Get() == Light::Type::Point)
                        {
                            float range = mState.Point.Range;
                            if (ImGui::DragFloat("Range", &range, 1.f, 0.f, 1000.f))
                            {
                                mState.Point.Range = range;
                                if (mCurrentLight)
                                    static_cast<PointLight*>(mCurrentLight)->SetRange(mState.Point.Range);
                            }

                            ImGui::Separator();

                            bool changed = false;
                            Vector3<float> position = mState.Point.Position;
                            changed |= ImGui::DragFloat("X", &position.X(), 0.1f, -1000.f, 1000.f, "%.3f");
                            changed |= ImGui::DragFloat("Y", &position.Y(), 0.1f, -1000.f, 1000.f, "%.3f");
                            changed |= ImGui::DragFloat("Z", &position.Z(), 0.1f, -1000.f, 1000.f, "%.3f");
                            if (changed)
                            {
                                mState.Point.Position = position;
                                if (mCurrentLight)
                                    static_cast<PointLight*>(mCurrentLight)->SetPosition(mState.Point.Position);
                            }
                        }
                        else if (mState.Type.Get() == Light::Type::Spot)
                        {
                            float range = mState.Spot.Range;
                            if (ImGui::DragFloat("Range", &range, 1.f, 0.f, 1000.f))
                            {
                                mState.Spot.Range = range;
                                if (mCurrentLight)
                                    static_cast<SpotLight*>(mCurrentLight)->SetRange(mState.Spot.Range);
                            }

                            ImGui::Separator();

                            bool changed = false;
                            Vector3<float> position = mState.Spot.Position;
                            changed |= ImGui::DragFloat("X", &position.X(), 0.1f, -1000.f, 1000.f, "%.3f");
                            changed |= ImGui::DragFloat("Y", &position.Y(), 0.1f, -1000.f, 1000.f, "%.3f");
                            changed |= ImGui::DragFloat("Z", &position.Z(), 0.1f, -1000.f, 1000.f, "%.3f");
                            if (changed)
                            {
                                mState.Spot.Position = position;
                                if (mCurrentLight)
                                    static_cast<SpotLight*>(mCurrentLight)->SetPosition(mState.Spot.Position);
                            }

                            ImGui::Separator();

                            float innerCutoff = mState.Spot.InnerCutoff.AsDegree();
                            if (ImGui::DragFloat("InnerCutoff", &innerCutoff, 0.1f, 0.f, 90.f, "%.1f"))
                            {
                                mState.Spot.InnerCutoff = Angle<float>::Degree(innerCutoff);
                                if (mCurrentLight)
                                    static_cast<SpotLight*>(mCurrentLight)->SetInnerCutoff(mState.Spot.InnerCutoff);
                            }

                            ImGui::Separator();

                            float outerCutoff = mState.Spot.OuterCutoff.AsDegree();
                            if (ImGui::DragFloat("OuterCutoff", &outerCutoff, 0.1f, 0.f, 90.f, "%.1f"))
                            {
                                mState.Spot.OuterCutoff = Angle<float>::Degree(outerCutoff);
                                if (mCurrentLight)
                                    static_cast<SpotLight*>(mCurrentLight)->SetOuterCutoff(mState.Spot.OuterCutoff);
                            }
                        }
                    }

                    ImGui::Separator();
                    if (mCreate)
                    {
                        if (ImGui::Button("Submit", ImVec2(150.f, 25.f)))
                        {
                            mState.Type.Then([&](Light::Type lightType) {
                                switch (lightType)
                                {
                                    case Light::Type::Directional:
                                        mCurrentLight = DirectionalLight::Create(mScene->shared_from_this(), mState.Name, mState.Directional.Direction, mState.Color, mState.Intensity);
                                        break;

                                    case Light::Type::Point:
                                        mCurrentLight = PointLight::Create(mScene->shared_from_this(), mState.Name, mState.Point.Range, mState.Color, mState.Point.Position, mState.Intensity);
                                        break;

                                    case Light::Type::Spot:
                                        mCurrentLight = SpotLight::Create(mScene->shared_from_this(), mState.Name, mState.Spot.Range, mState.Color, mState.Point.Position, mState.Intensity);
                                        break;
                                }

                                mCreate = false;
                            });
                        }

                        ImGui::SameLine();
                        if (ImGui::Button("Cancel", ImVec2(150.f, 25.f)))
                        {
                            mState = {};
                            mCreate = false;
                        }
                    }
                    else
                    {
                        if (ImGui::Button("Remove", ImVec2(150.f, 25.f)))
                        {
                            mScene->RemoveLight(mCurrentLight);

                            mState = {};
                            mCreate = false;
                            mCurrentLight = nullptr;
                        }
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

        struct LightState
        {
            Optional<Light::Type> Type;
            String Name = String::Empty;
            LinearColor Color = LinearColor::White;
            float Intensity = 100.f;
            struct
            {
                Vector3<float> Direction = Vector3<float>::Zero();
            } Directional;

            struct
            {
                float Range = 0.f;
                Vector3<float> Position = Vector3<float>::Zero();
            } Point;

            struct
            {
                float Range = 0.f;
                Vector3<float> Position = Vector3<float>::Zero();
                Angle<float> InnerCutoff = Angle<float>::Zero();
                Angle<float> OuterCutoff = Angle<float>::Zero();
            } Spot;
        };

        Scene* mScene;
        bool mCreate;
        LightState mState;
        Light* mCurrentLight = nullptr;
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
