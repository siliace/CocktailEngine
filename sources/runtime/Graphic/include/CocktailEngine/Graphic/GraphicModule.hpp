#pragma once

#include <CocktailEngine/Graphic/GraphicModuleGenerated.hpp>

/// Entry point of the CocktailEngine::Graphic module.
///
/// Module-specific code goes here. What the build knows
/// about the module comes from the generated base class.
class GraphicModule : public GraphicModuleGenerated
{
public:
    /// Brings the module up. Called once, after it is loaded.
    void Startup() override;

    /// Tears the module down. Called once, before it is unloaded.
    void Shutdown() override;
};
