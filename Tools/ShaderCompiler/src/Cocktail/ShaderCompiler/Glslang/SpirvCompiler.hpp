#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVCOMPILER_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVCOMPILER_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/ShaderCompiler/Compiler.hpp>
#include <Cocktail/ShaderCompiler/CompilerCreateInfo.hpp>

namespace Ck::Tools::ShaderCompiler
{
	class SpirvCompiler : public Inherit<SpirvCompiler, Object, Compiler>
	{
	public:

		/**
		 * \brief 
		 * \param createInfo 
		 */
		SpirvCompiler(const CompilerCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<ShaderObject> CreateShaderObject(const ShaderObjectCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param type 
		 * \return 
		 */
		Ref<Program> CreateProgram(Renderer::ShaderProgramType type) override;

		/**
		 * \brief 
		 * \return 
		 */
		ShaderBinaryFormat GetOutputFormat() const override;

	private:

		TargetApi mTargetApi;
		ShaderBinaryFormat mOutputFormat;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVCOMPILER_HPP
