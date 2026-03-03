#ifndef COCKTAIL_SPONZA_SCENEMENUMODULE_HPP
#define COCKTAIL_SPONZA_SCENEMENUMODULE_HPP

#include <Cocktail/Sponza/ImUi/Menu/PanelControllerMenuModule.hpp>

namespace Ck
{
    class Scene;


    class SceneMenuModule : public ImUi::PanelControllerMenuModule
    {
    public:

        explicit SceneMenuModule(Scene* scene);

        void Attach() override;

        const AnsiChar* GetName() const override;

    private:

        Scene* mScene;
    };
}

#endif // COCKTAIL_SPONZA_SCENEMENUMODULE_HPP
