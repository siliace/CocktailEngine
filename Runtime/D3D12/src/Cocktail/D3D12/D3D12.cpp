#include <Cocktail/D3D12/D3D12.hpp>
#include <Cocktail/D3D12/RenderDevice.hpp>

namespace Ck::D3D12
{
	COCKTAIL_DEFINE_LOG_CATEGORY(D3D12LogCategory);

    UniquePtr<Renderer::RenderDevice> CreateRenderDevice()
    {
        return MakeUnique<RenderDevice>();
    }
}
