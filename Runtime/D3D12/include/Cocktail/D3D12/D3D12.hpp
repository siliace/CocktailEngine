#ifndef COCKTAIL_D3D12_D3D12_HPP
#define COCKTAIL_D3D12_D3D12_HPP

#include <Cocktail/Core/Log/LogCategory.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>

#include <Cocktail/D3D12/Export.hpp>

namespace Ck
{
    namespace Renderer
    {
        class RenderDevice;
    }

    namespace D3D12
    {
        COCKTAIL_DECLARE_LOG_CATEGORY(D3D12LogCategory, LogLevel::Info);

        COCKTAIL_D3D12_API UniquePtr<Renderer::RenderDevice> CreateRenderDevice();
    }
}

#endif // COCKTAILENGINE_D3D12_HPP
