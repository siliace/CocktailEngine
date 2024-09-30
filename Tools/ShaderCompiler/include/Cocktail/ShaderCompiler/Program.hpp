#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_PROGRAM_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_PROGRAM_HPP

#include <Cocktail/Renderer/Shader/ShaderProgramType.hpp>

#include <Cocktail/ShaderCompiler/ShaderObject.hpp>

namespace Ck::Tools::ShaderCompiler
{
	/**
	 * \brief 
	 */
	class Program : public Implements<Program, Interface>
	{
	public:

		/**
		 * \brief
		 * \param shaderObject
		 */
		virtual void AddShaderObject(Ref<ShaderObject> shaderObject) = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual bool Link() = 0;

		/**
		 * \brief
		 * \param stage
		 * \return
		 */
		virtual const std::vector<Uint32>& GetStageBinary(Renderer::ShaderType stage) const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Renderer::ShaderProgramType GetType() const = 0;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_PROGRAM_HPP
