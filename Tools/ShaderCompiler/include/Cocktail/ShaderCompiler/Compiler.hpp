#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_COMPILER_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_COMPILER_HPP

#include <Cocktail/ShaderCompiler/Program.hpp>
#include <Cocktail/ShaderCompiler/ShaderBinaryFormat.hpp>

namespace Ck::Tools::ShaderCompiler
{
	/**
	 * \brief 
	 */
	class Compiler : public Implements<Compiler, Interface>
	{
	public:

		/**
		 * \brief
		 * \param createInfo
		 * \return
		 */
		virtual Ref<ShaderObject> CreateShaderObject(const ShaderObjectCreateInfo& createInfo) = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Ref<Program> CreateProgram(Renderer::ShaderProgramType type) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual ShaderBinaryFormat GetOutputFormat() const = 0;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_COMPILER_HPP
