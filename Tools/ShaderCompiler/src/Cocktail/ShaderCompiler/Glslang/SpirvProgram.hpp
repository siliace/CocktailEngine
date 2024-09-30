#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVPROGRAM_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVPROGRAM_HPP

#include <memory>

#include <glslang/Public/ShaderLang.h>

#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/ShaderCompiler/Program.hpp>
#include <Cocktail/ShaderCompiler/Glslang/SpirvShaderObject.hpp>

namespace Ck::Tools::ShaderCompiler
{
	class SpirvProgram : public Inherit<SpirvProgram, Object, Program>
	{
	public:

		explicit SpirvProgram(Renderer::ShaderProgramType type);

		void AddShaderObject(Ref<ShaderObject> shaderObject) override;

		bool Link() override;

		const std::vector<Uint32>& GetStageBinary(Renderer::ShaderType stage) const override;

		Renderer::ShaderProgramType GetType() const override;

	private:

		Renderer::ShaderProgramType mType;
		std::unique_ptr<glslang::TProgram> mProgram;
		std::vector<Ref<SpirvShaderObject>> mShaderObjects;
		EnumMap<Renderer::ShaderType, std::vector<Uint32>> mStageBinaries;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVPROGRAM_HPP
