#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>
#include <Cocktail/Renderer/Shader/VertexAttributeLocation.hpp>

namespace Ck
{
    class COCKTAIL_GRAPHIC_API MaterialProgramVariant
    {
    public:

        MaterialProgramVariant(SharedPtr<MaterialProgramInterface>, SharedPtr<Renderer::ShaderProgram> shaderProgram);

        Renderer::VertexAttributeLocation* GetVertexAttributeLocation(VertexAttributeSemantic semantic) const;

        const Renderer::UniformSlot* GetSlot(ShaderBindingDomain domain, BindingSlot bindingSlot) const;

        Renderer::ShaderProgram* GetShaderProgram() const;

    private:

        using SlotIdentifier = CompositeKey<ShaderBindingDomain, BindingSlot>;

        SharedPtr<MaterialProgramInterface> mInterface;
        SharedPtr<Renderer::ShaderProgram> mShaderProgram;
        EnumMap<VertexAttributeSemantic, Renderer::VertexAttributeLocation*> mVertexAttributeLocations;
        HashMap<SlotIdentifier, Renderer::UniformSlot*> mSlots;
    };
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP
