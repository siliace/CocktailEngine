#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramInterface.hpp>

namespace Ck
{
    MaterialProgramInterface::Builder::Builder()
    {
        mInterface = MakeShared<MaterialProgramInterface>();
    }

    MaterialProgramInterface::Builder& MaterialProgramInterface::Builder::SetVertexAttribute(VertexAttributeSemantic attribute, AsciiString name)
    {
        mInterface->mVertexAttributes[attribute].Name = Move(name);

        return *this;
    }

    MaterialProgramInterface::Builder& MaterialProgramInterface::Builder::SetBindingSlot(ShaderBindingDomain domain, BindingSlot bindingSlot, AsciiString name, Renderer::DescriptorType type, Renderer::StaticSampler sampler)
    {
        if (mInterface->mDomainInterfaces[domain] == nullptr)
            mInterface->mDomainInterfaces[domain] = MakeUnique<BindingDomainInterface>();

        mInterface->mDomainInterfaces[domain]->SetSlot(bindingSlot, Move(name), type, sampler);

        return *this;
    }

    SharedPtr<MaterialProgramInterface> MaterialProgramInterface::Builder::Get() const
    {
        return mInterface;
    }

    const VertexInterface* MaterialProgramInterface::GetVertexInterface(VertexAttributeSemantic semantic)
    {
        return mVertexAttributes.TryGet(semantic).Map([](const VertexInterface& interface) {
            return &interface;
        }).GetOr(nullptr);
    }

    const BindingDomainInterface* MaterialProgramInterface::GetBindingDomainInterface(ShaderBindingDomain domain) const
    {
        return mDomainInterfaces[domain].Get();
    }
}
