#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>

namespace Ck
{
    MaterialProgramInterface::Builder::Builder()
    {
        mInterface = std::make_shared<MaterialProgramInterface>();
    }

    MaterialProgramInterface::Builder& MaterialProgramInterface::Builder::SetVertexAttribute(VertexAttributeSemantic attribute, AsciiString name)
    {
        mInterface->mVertexAttributes[attribute].Name = std::move(name);

        return *this;
    }

    MaterialProgramInterface::Builder& MaterialProgramInterface::Builder::SetBindingSlot(ShaderBindingDomain domain, BindingSlot bindingSlot, AsciiString name, Renderer::DescriptorType type, Renderer::StaticSampler sampler)
    {
        if (mInterface->mDomainInterfaces[domain] == nullptr)
            mInterface->mDomainInterfaces[domain] = MakeUnique<BindingDomainInterface>();

        mInterface->mDomainInterfaces[domain]->SetSlot(bindingSlot, std::move(name), type, sampler);

        return *this;
    }

    std::shared_ptr<MaterialProgramInterface> MaterialProgramInterface::Builder::Get() const
    {
        return mInterface;
    }

    const VertexInterface* MaterialProgramInterface::GetVertexInterface(VertexAttributeSemantic semantic)
    {
        if (auto it = mVertexAttributes.find(semantic); it != mVertexAttributes.end())
            return &it->second;

        return nullptr;
    }

    const BindingDomainInterface* MaterialProgramInterface::GetBindingDomainInterface(ShaderBindingDomain domain) const
    {
        return mDomainInterfaces[domain].Get();
    }
}
