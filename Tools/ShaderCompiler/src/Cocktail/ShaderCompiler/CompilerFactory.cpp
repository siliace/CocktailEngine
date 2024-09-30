#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/ShaderCompiler/CompilerFactory.hpp>
#include <Cocktail/ShaderCompiler/Glslang/SpirvCompiler.hpp>

namespace Ck::Tools::ShaderCompiler
{
	COCKTAIL_DECLARE_EXCEPTION_BASE(IncompatibleShaderFormat, "Incompatible output shader format", std::invalid_argument);

	namespace
	{
		[[noreturn]] void ThrowIncompatibleShaderFormat(ShaderBinaryFormat format, TargetApi targetApi)
		{
			throw IncompatibleShaderFormat(
				"ShaderBinaryFormat {} is incompatible with TargetApi {}",
				Enum<ShaderBinaryFormat>::ToString(format),
				Enum<TargetApi>::ToString(targetApi)
			);
		}
	}

	Ref<Compiler> CompilerFactory::CreateCompiler(const CompilerCreateInfo& createInfo) const
	{
		TargetApi targetApi = createInfo.TargetApi;
		ShaderBinaryFormat format = createInfo.OutputFormat;

		if (targetApi == TargetApi::DirectX12)
		{
			if (format != ShaderBinaryFormat::DXIL_SM_6)
				ThrowIncompatibleShaderFormat(format, targetApi);

			return nullptr;
		}
		else 
		{
			if (createInfo.OutputFormat == ShaderBinaryFormat::DXIL_SM_6)
				ThrowIncompatibleShaderFormat(createInfo.OutputFormat, targetApi);

			if (targetApi == TargetApi::Vulkan_1_0 && createInfo.OutputFormat != ShaderBinaryFormat::Spv_1_0)
				Log::Warning("TargetApi {} needs an extension to support format {}, compiled shader might not supported everywhere", Enum<TargetApi>::ToString(targetApi), Enum<ShaderBinaryFormat>::ToString(format));

			return SpirvCompiler::New(createInfo);
		}
	}
}
