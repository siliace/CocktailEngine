#ifndef COCKTAIL_RENDERER_SHADER_SHADERPROGRAMTYPE_HPP
#define COCKTAIL_RENDERER_SHADER_SHADERPROGRAMTYPE_HPP

#include <Cocktail/Renderer/Export.hpp>
#include <Cocktail/Renderer/Shader/ShaderType.hpp>

namespace Ck::Renderer
{
    /**
     * \brief Enumeration of possible shader program types
     */
    enum class ShaderProgramType
    {
        /**
         * \brief Graphic shader program type
         */
        Graphic,
      
        /**
         * \brief Compute shader program type
         */
        Compute,
    };

    /**
     * \brief Tell whether a type of shader will be supported by a given type of ShaderProgram
     * \param programType
     * \param shaderType
     * \return
     */
    COCKTAIL_RENDERER_API bool ProgramSupportShader(ShaderProgramType programType, ShaderType shaderType);
}

#endif // COCKTAIL_RENDERER_SHADER_SHADERPROGRAMTYPE_HPP
