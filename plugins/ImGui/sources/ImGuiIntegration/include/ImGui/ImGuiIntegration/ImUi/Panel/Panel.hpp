#ifndef IMGUI_IMGUIINTEGRATION_PANEL_HPP
#define IMGUI_IMGUIINTEGRATION_PANEL_HPP

#include <CocktailEngine/Core/Signal/Observable.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

#include <ImGui/ImGuiIntegration/Export.hpp>

namespace Ck::ImUi
{
    class IMGUI_IMGUIINTEGRATION_API Panel : public Observable
    {
    public:

        Panel(const Panel& other) = delete;
        Panel(Panel&& other) noexcept = default;
        Panel& operator=(const Panel& other) = delete;
        Panel& operator=(Panel&& other) noexcept = default;

        virtual void Attach();

        virtual void Update(const Duration& deltaTime);

        virtual void Render() = 0;

        bool IsOpen() const;
        void SetOpen(bool open);

        virtual const AnsiChar* GetName() const = 0;

    protected:

        Panel();

        bool mOpen;
    };

}

#endif // IMGUI_IMGUIINTEGRATION_PANEL_HPP
