#ifndef COCKTAILENGINE_RENDERER_SHADER_SHADERPROGRAMTYPE_HPP
#define COCKTAILENGINE_RENDERER_SHADER_SHADERPROGRAMTYPE_HPP

#include <CocktailEngine/Renderer/Export.hpp>
#include <CocktailEngine/Renderer/Shader/ShaderType.hpp>

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
    COCKTAILENGINE_RENDERER_API bool ProgramSupportShader(ShaderProgramType programType, ShaderType shaderType);
}

#endif // COCKTAILENGINE_RENDERER_SHADER_SHADERPROGRAMTYPE_HPP
