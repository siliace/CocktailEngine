#include <Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp>

#include <Cocktail/Renderer/Shader/Shader.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
    Optional<unsigned int> BindingDomainInterface::GetSlotInterfaceIndex(BindingSlot bindingSlot) const
    {
        return mBindingSlotInterfaces.FindIndexIf([&](BindingSlotInterface value) {
            return value.BindingSlot == bindingSlot;
        });
    }

    unsigned int BindingDomainInterface::GetSlotInterfaceCount() const
    {
        return mBindingSlotInterfaces.GetSize();
    }

    const SlotInterface& BindingDomainInterface::GetSlotInterface(unsigned int index) const
    {
        assert(index < mBindingSlotInterfaces.GetSize());
        return mBindingSlotInterfaces[index].Interface;
    }

    void BindingDomainInterface::SetSlot(BindingSlot bindingSlot, AsciiString name, Renderer::DescriptorType type, Renderer::StaticSampler sampler)
    {
        auto bindingSlotIndexToInterface = [&](unsigned int slotInterfaceIndex) -> SlotInterface& {
            return mBindingSlotInterfaces[slotInterfaceIndex].Interface;
        };

        auto createBindingSlotInterface = [&]() -> SlotInterface& {
            BindingSlotInterface bindingSlotInterface;
            bindingSlotInterface.BindingSlot = bindingSlot;

            return mBindingSlotInterfaces.Emplace(bindingSlotInterface).Interface;
        };

        SlotInterface& slotInterface = GetSlotInterfaceIndex(bindingSlot).Map(bindingSlotIndexToInterface).GetOrElse(createBindingSlotInterface);
        slotInterface.Name = std::move(name);
        slotInterface.Type = type;
        slotInterface.Sampler = sampler;
    }

    MaterialProgramVariant::MaterialProgramVariant(std::shared_ptr<MaterialProgramInterface> interface, std::shared_ptr<Renderer::ShaderProgram> shaderProgram) :
        mInterface(std::move(interface)),
        mShaderProgram(std::move(shaderProgram))
    {
        assert(mShaderProgram);

        std::shared_ptr<Renderer::Shader> vertexStage = mShaderProgram->GetStage(Renderer::ShaderType::Vertex);
        assert(vertexStage);

        for (VertexAttributeSemantic vertexAttribute : Enum<VertexAttributeSemantic>::Values)
        {
            if (const VertexInterface* vertexInterface = mInterface->GetVertexInterface(vertexAttribute))
                mVertexAttributeLocations[vertexAttribute] = vertexStage->FindInputAttribute(vertexInterface->Name);
        }

        for (ShaderBindingDomain domain : Enum<ShaderBindingDomain>::Values)
        {
            const BindingDomainInterface* bindingDomainInterface = mInterface->GetBindingDomainInterface(domain);
            if (!bindingDomainInterface)
                continue;

            for (unsigned int i = 0; i < bindingDomainInterface->GetSlotInterfaceCount(); i++)
            {
                const SlotInterface& slotInterface = bindingDomainInterface->GetSlotInterface(i);
                if (Renderer::UniformSlot* slot = mShaderProgram->FindUniformSlot(slotInterface.Name))
                {
                    if (slot->GetDescriptorType() != slotInterface.Type)
                        continue;

                    mSlots.insert_or_assign(SlotIdentifier{ domain, i }, slot);
                }
            }
        }
    }

    const Renderer::UniformSlot* MaterialProgramVariant::GetSlot(ShaderBindingDomain domain, BindingSlot bindingSlot) const
    {
        assert(bindingSlot != InvalidBindingSlot);

        const BindingDomainInterface* domainInterface = mInterface->GetBindingDomainInterface(domain);
        if (!domainInterface)
            return nullptr;

        Optional<unsigned int> slotInterfaceIndex = domainInterface->GetSlotInterfaceIndex(bindingSlot);
        if (!slotInterfaceIndex.IsEmpty())
        {
            SlotIdentifier id{ domain, slotInterfaceIndex.Get() };
            if (auto it = mSlots.find(id); it != mSlots.end())
                return it->second;
        }

        return nullptr;
    }

    Renderer::VertexAttributeLocation* MaterialProgramVariant::GetVertexAttributeLocation(VertexAttributeSemantic semantic) const
    {
        return mVertexAttributeLocations[semantic];
    }

    Renderer::ShaderProgram* MaterialProgramVariant::GetShaderProgram() const
    {
        return mShaderProgram.get();
    }
}
