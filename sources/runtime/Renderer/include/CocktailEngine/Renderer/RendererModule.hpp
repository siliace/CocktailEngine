#pragma once

#include <CocktailEngine/Renderer/RendererModuleGenerated.hpp>

/// Entry point of the CocktailEngine::Renderer module.
///
/// Module-specific code goes here. What the build knows
/// about the module comes from the generated base class.
class RendererModule : public RendererModuleGenerated
{
public:
    /// Brings the module up. Called once, after it is loaded.
    void Startup() override;

    /// Tears the module down. Called once, before it is unloaded.
    void Shutdown() override;
};
