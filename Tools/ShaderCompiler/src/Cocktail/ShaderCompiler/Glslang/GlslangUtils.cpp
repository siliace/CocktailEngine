#include <Cocktail/ShaderCompiler/Glslang/GlslangUtils.hpp>

namespace Ck::Tools::ShaderCompiler
{
	EShLanguage ToGlslangType(Renderer::ShaderType stage)
	{
		switch (stage)
		{
		case Renderer::ShaderType::Vertex: return EShLangVertex;
		case Renderer::ShaderType::TessellationControl: return EShLangTessControl;
		case Renderer::ShaderType::TessellationEvaluation: return EShLangTessEvaluation;
		case Renderer::ShaderType::Geometry: return EShLangGeometry;
		case Renderer::ShaderType::Fragment: return EShLangFragment;
		case Renderer::ShaderType::Compute: return EShLangCompute;
		}

		COCKTAIL_UNREACHABLE();
	}

	glslang::EShSource ToGlslangType(Language language)
	{
		switch (language)
		{
		case Language::Glsl: return glslang::EShSource::EShSourceGlsl;
		case Language::Hlsl: return glslang::EShSource::EShSourceHlsl;
		}

		COCKTAIL_UNREACHABLE();
	}

	glslang::EShTargetClientVersion ToGlslangType(TargetApi targetApi)
	{
		switch (targetApi)
		{
		case TargetApi::Vulkan_1_0: return glslang::EShTargetVulkan_1_0;
		case TargetApi::Vulkan_1_1: return glslang::EShTargetVulkan_1_1;
		case TargetApi::Vulkan_1_2: return glslang::EShTargetVulkan_1_2;
		case TargetApi::Vulkan_1_3: return glslang::EShTargetVulkan_1_3;
		case TargetApi::OpenGL_4_5: return glslang::EShTargetOpenGL_450;
		}

		COCKTAIL_UNREACHABLE();
	}

	glslang::EShTargetLanguageVersion ToGlslangType(ShaderBinaryFormat format)
	{
		switch (format)
		{
		case ShaderBinaryFormat::Spv_1_0: return glslang::EShTargetSpv_1_0;
		case ShaderBinaryFormat::Spv_1_1: return glslang::EShTargetSpv_1_1;
		case ShaderBinaryFormat::Spv_1_2: return glslang::EShTargetSpv_1_2;
		case ShaderBinaryFormat::Spv_1_3: return glslang::EShTargetSpv_1_3;
		case ShaderBinaryFormat::Spv_1_4: return glslang::EShTargetSpv_1_4;
		case ShaderBinaryFormat::Spv_1_5: return glslang::EShTargetSpv_1_5;
		case ShaderBinaryFormat::Spv_1_6: return glslang::EShTargetSpv_1_6;
		}

		COCKTAIL_UNREACHABLE();
	}
}
