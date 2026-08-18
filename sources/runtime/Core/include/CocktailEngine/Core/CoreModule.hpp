#pragma once

#include <CocktailEngine/Core/CoreModuleGenerated.hpp>

/// Entry point of the CocktailEngine::Core module.
///
/// Module-specific code goes here. What the build knows
/// about the module comes from the generated base class.
class CoreModule : public CoreModuleGenerated
{
public:
    /// Brings the module up. Called once, after it is loaded.
    void Startup() override;

    /// Tears the module down. Called once, before it is unloaded.
    void Shutdown() override;
};
