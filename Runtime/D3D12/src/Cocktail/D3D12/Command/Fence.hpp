#ifndef COCKTAIL_D3D12_COMMAND_FENCE_HPP
#define COCKTAIL_D3D12_COMMAND_FENCE_HPP

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Renderer/Command/Synchronization/Fence.hpp>
#include <Cocktail/Renderer/Command/Synchronization/FenceCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class Fence : public Renderer::Fence, public std::enable_shared_from_this<Fence>
    {
    public:

        Fence(RenderDevice* renderDevice, const Renderer::FenceCreateInfo& createInfo);
        ~Fence();
        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        void StartSignal(ID3D12CommandQueue* commandQueue);
        void Wait() override;
        bool Wait(const Duration& timeout) override;
        bool IsSignaled() const override;
        void Reset() override;
        Signal<>& OnSignaled() override;

    private:

        RenderDevice* mRenderDevice;
        Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
        Signal<> mOnSignaled;
        Uint64 mNextValue;
        Uint64 mWaitValue;
        HANDLE mEvent;
    };
}

#endif // COCKTAIL_D3D12_COMMAND_FENCE_HPP
