#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/ShaderCompiler/Glslang/GlslShaderIncluder.hpp>

namespace Ck::Tools::ShaderCompiler
{
	GlslShaderIncluder::GlslShaderIncluder(std::filesystem::path shaderDirectory):
		mShaderDirectory(std::move(shaderDirectory))
	{
		/// Nothing
	}

	glslang::TShader::Includer::IncludeResult* GlslShaderIncluder::includeSystem(const char* headerName, const char* includerName, std::size_t inclusionDepth)
	{
		return nullptr;
	}

	glslang::TShader::Includer::IncludeResult* GlslShaderIncluder::includeLocal(const char* headerName, const char* includerName, std::size_t inclusionDepth)
	{
		std::filesystem::path headerPath = mShaderDirectory / headerName;
		if (auto it = mIncludes.find(headerPath.string()); it != mIncludes.end())
			return it->second;

		if (!Storage::IsFile(headerPath))
			return nullptr;

		std::string headerContent = FileUtils::ReadFile(headerPath);
		auto [it, inserted] = mSources.emplace(std::make_pair(headerPath.string(), std::move(headerContent)));
		if (!inserted)
			return nullptr;

		IncludeResult* result = new IncludeResult(headerPath.string(), it->second.c_str(), it->second.length(), this);
		mIncludes.emplace(std::make_pair(headerPath.string(), result));

		return result;
	}

	void GlslShaderIncluder::releaseInclude(IncludeResult* includeResult)
	{
		if (auto it = mIncludes.find(includeResult->headerName); it != mIncludes.end())
			mIncludes.erase(it);

		if (auto it = mSources.find(includeResult->headerName); it != mSources.end())
			mSources.erase(it);

		delete includeResult;
	}
}
