#include <Cocktail/D3D12/Command/Fence.hpp>
#include <Cocktail/D3D12/RenderDevice.hpp>

namespace Ck::D3D12
{
    Fence::Fence(RenderDevice* renderDevice, const Renderer::FenceCreateInfo& createInfo) :
        mRenderDevice(renderDevice),
        mWaitValue(0),
        mNextValue(0)
    {
        if (createInfo.Signaled)
            mNextValue = mWaitValue = 1;

        mRenderDevice->GetHandle()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
        mEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }

    Fence::~Fence()
    {
        if (mEvent)
            CloseHandle(mEvent);
    }

    void Fence::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* Fence::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    void Fence::StartSignal(ID3D12CommandQueue* commandQueue)
    {
        mNextValue++;
        mWaitValue = mNextValue;

        commandQueue->Signal(mFence.Get(), mNextValue);
    }

    void Fence::Wait()
    {
        Wait(Duration::Infinite());
    }

    bool Fence::Wait(const Duration& timeout)
    {
        if (IsSignaled())
            return true;

        mFence->SetEventOnCompletion(mWaitValue, mEvent);

        DWORD result = WaitForSingleObject(mEvent, timeout.GetCount(TimeUnit::Milliseconds()));
        if (result != WAIT_OBJECT_0)
            return false;

        mOnSignaled.Emit();
        return true;
    }

    bool Fence::IsSignaled() const
    {
        return mFence->GetCompletedValue() >= mWaitValue;
    }

    void Fence::Reset()
    {
        mWaitValue = 0;
    }

    Signal<>& Fence::OnSignaled()
    {
        return mOnSignaled;
    }
}
