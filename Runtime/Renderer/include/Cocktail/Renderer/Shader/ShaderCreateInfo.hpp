#ifndef COCKTAIL_RENDERER_SHADER_SHADERCREATEINFO_HPP
#define COCKTAIL_RENDERER_SHADER_SHADERCREATEINFO_HPP

#include <Cocktail/Core/Cocktail.hpp>

#include <Cocktail/Renderer/Shader/ShaderType.hpp>

namespace Ck::Renderer
{
    /**
     * \brief Shader descriptor structure
     * Describe the set of parameters to create a Shader
     * \see RenderDevice::CreateShader
     */
    struct ShaderCreateInfo
    {
	    /**
		 * \brief Specifies the type of Shader to create
		 */
        ShaderType Type = ShaderType::Vertex;

        /**
         * \brief The length of \p Code buffer
         */
        std::size_t CodeLength = 0;
    	
		/** 
		 * \brief The code of the Shader to create
		 */
        const Uint32* Code = nullptr;

        /**
         * \brief Specifies the name of the Shader to create
         * If the extension Debug is not supported by the RenderDevice creating the ShaderProgram, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_SHADER_SHADERCREATEINFO_HPP