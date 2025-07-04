#ifndef COCKTAIL_RENDERER_SHADER_SHADERPROGRAMCREATEINFO_HPP
#define COCKTAIL_RENDERER_SHADER_SHADERPROGRAMCREATEINFO_HPP

#include <Cocktail/Core/Enum.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgramType.hpp>
#include <Cocktail/Renderer/Texture/StaticSampler.hpp>

namespace Ck::Renderer
{
    class Shader;

    struct StaticSamplerInfo
    {
	    /**
         * \brief 
         */
        std::string_view Member;

	    /**
         * \brief 
         */
        StaticSampler Sampler;
    };

    /**
     * \brief ShaderProgram descriptor structure
     * Describe the set of parameters to create a ShaderProgram
     * \see RenderDevice::CreateShaderProgram
     */
    struct ShaderProgramCreateInfo
    {
        /**
         * \brief Specifies the type of ShaderProgram to create
         */
        ShaderProgramType Type = ShaderProgramType::Graphic;

        /**
         * \brief Specifies the number of Shader linked to the ShaderProgram to create
         */
        unsigned int ShaderCount = 0;

        /**
         * \brief Specifies Shader linked to the ShaderProgram to create
         * Every ShaderType must be unique within the list of Shader
         * Every ShaderType must be compatible with the \p Type of the ShaderProgram to create
         * \see ProgramSupportShader
         */
        std::shared_ptr<Shader> Shaders[Enum<ShaderType>::ValueCount];

        /**
         * \brief Specifies the number of static samplers
         */
        unsigned int StaticSamplerCount = 0;

        /**
         * \brief Specifies the static samplers to bind to Sampler/Texture samplers descriptors
         */
        const StaticSamplerInfo* StaticSamplers = nullptr;

        /**
         * \brief Specifies the name of the ShaderProgram to create
         * If the extension Debug is not supported by the RenderDevice creating the ShaderProgram, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_SHADER_SHADERPROGRAMCREATEINFO_HPP