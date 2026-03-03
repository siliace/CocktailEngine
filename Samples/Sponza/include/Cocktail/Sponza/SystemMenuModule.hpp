#ifndef COCKTAIL_SPONZA_SYSTEMMENUMODULE_HPP
#define COCKTAIL_SPONZA_SYSTEMMENUMODULE_HPP

#include <Cocktail/Sponza/ImUi/Menu/PanelControllerMenuModule.hpp>

namespace Ck
{
    class SystemMenuModule : public ImUi::PanelControllerMenuModule
    {
    public:

        void Attach() override;

        const AnsiChar* GetName() const override;
    };
}

#endif // COCKTAIL_SPONZA_SYSTEMMENUMODULE_HPP
