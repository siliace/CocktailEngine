#ifndef COCKTAIL_SPONZA_IMGUI_PANEL_HPP
#define COCKTAIL_SPONZA_IMGUI_PANEL_HPP

#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck::ImUi
{
    class Panel : public Observable
    {
    public:

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

#endif // COCKTAIL_SPONZA_IMGUI_PANEL_HPP
