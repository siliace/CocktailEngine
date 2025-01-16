#ifndef COCKTAIL_RENDERER_SHADER_SHADERPROGRAM_HPP
#define COCKTAIL_RENDERER_SHADER_SHADERPROGRAM_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramType.hpp>

namespace Ck::Renderer
{
    class Shader;
    class UniformSlot;

	/**
     * \brief 
     */
    class ShaderProgram : public Implements<ShaderProgram, RenderDeviceObject>
    {
    public:

	    /**
         * \brief 
         * \return 
         */
        virtual ShaderProgramType GetType() const = 0;

        /**
         * \brief 
         * \param type 
         * \return 
         */
        virtual Ref<Shader> GetStage(ShaderType type) const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual std::size_t GetUniformSlotCount() const = 0;

	    /**
         * \brief 
         * \param slots 
         * \param slotCount 
         * \param firstSlot 
         * \return 
         */
        virtual std::size_t GetUniformSlots(UniformSlot** slots, std::size_t slotCount, std::size_t firstSlot) const = 0;
    };
}

#endif // COCKTAIL_RENDERER_SHADER_SHADERPROGRAM_HPP
