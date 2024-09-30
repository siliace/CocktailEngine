#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_TARGETAPI_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_TARGETAPI_HPP

namespace Ck::Tools::ShaderCompiler
{
	enum class TargetApi
	{
		Vulkan_1_0,
		Vulkan_1_1,
		Vulkan_1_2,
		Vulkan_1_3,
		OpenGL_4_5,
		OpenGL_4_6 = OpenGL_4_5,
		DirectX12,
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_TARGETAPI_HPP
