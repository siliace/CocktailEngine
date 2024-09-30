#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_GLSLSHADERINCLUDER_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_GLSLSHADERINCLUDER_HPP

#include <filesystem>
#include <unordered_map>

#include <glslang/Public/ShaderLang.h>

namespace Ck::Tools::ShaderCompiler
{
	class GlslShaderIncluder : public glslang::TShader::Includer
	{
	public:

		explicit GlslShaderIncluder(std::filesystem::path shaderDirectory);

		IncludeResult* includeSystem(const char* headerName, const char* includerName, std::size_t inclusionDepth) override;

		IncludeResult* includeLocal(const char* headerName, const char* includerName, std::size_t inclusionDepth) override;

		void releaseInclude(IncludeResult* includeResult) override;

	private:

		std::filesystem::path mShaderDirectory;
		std::unordered_map<std::string, std::string> mSources;
		std::unordered_map<std::string, IncludeResult*> mIncludes;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_GLSLSHADERINCLUDER_HPP
