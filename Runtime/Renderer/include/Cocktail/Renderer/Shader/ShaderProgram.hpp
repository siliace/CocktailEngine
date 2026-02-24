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
    class ShaderProgram : public RenderDeviceObject
    {
    public:

	    /**
         * \brief
         *
         * \return 
         */
        virtual ShaderProgramType GetType() const = 0;

        /**
         * \brief
         *
         * \param type
         *
         * \return 
         */
        virtual std::shared_ptr<Shader> GetStage(ShaderType type) const = 0;

	    /**
         * \brief Find a UniformSlot by its name in the shader's sources
         *
         * \param name The name of the uniform
         *
         * \return The UniformSlot if found, nullptr otherwise
         */
        virtual UniformSlot* FindUniformSlot(AsciiStringView name) const = 0;

        /**
         * \brief
         *
         * \return
         */
        virtual unsigned int GetUniformSlotCount() const = 0;

        /**
         * \brief
         *
         * \param slots
         * \param slotCount
         * \param firstSlot
         *
         * \return
         */
        virtual unsigned int GetUniformSlots(UniformSlot** slots, unsigned int slotCount, unsigned int firstSlot) const = 0;
    };
}

#endif // COCKTAIL_RENDERER_SHADER_SHADERPROGRAM_HPP
