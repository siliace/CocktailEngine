#include <Cocktail/Core/Enum.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>

namespace Ck::Vulkan
{
	VkComponentSwizzle ToVkType(Renderer::TextureSwizzleValue swizzle)
	{
		switch (swizzle)
		{
		case Renderer::TextureSwizzleValue::Identity: return VK_COMPONENT_SWIZZLE_IDENTITY;
		case Renderer::TextureSwizzleValue::Zero: return VK_COMPONENT_SWIZZLE_ZERO;
		case Renderer::TextureSwizzleValue::One: return VK_COMPONENT_SWIZZLE_ONE;
		case Renderer::TextureSwizzleValue::Red: return VK_COMPONENT_SWIZZLE_R;
		case Renderer::TextureSwizzleValue::Green: return VK_COMPONENT_SWIZZLE_G;
		case Renderer::TextureSwizzleValue::Blue: return VK_COMPONENT_SWIZZLE_B;
		case Renderer::TextureSwizzleValue::Alpha: return VK_COMPONENT_SWIZZLE_A;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	Renderer::ColorSpace FromVkType(VkColorSpaceKHR colorSpace)
	{
		switch (colorSpace)
		{
		case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
			return Renderer::ColorSpace::Srgb;

		case VK_COLOR_SPACE_HDR10_ST2084_EXT:
			return Renderer::ColorSpace::Hdr10;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkColorSpaceKHR ToVkType(Renderer::ColorSpace colorSpace)
	{
		switch (colorSpace)
		{
		case Renderer::ColorSpace::Srgb:
			return VK_COLORSPACE_SRGB_NONLINEAR_KHR;

		case Renderer::ColorSpace::Hdr10:
			return VK_COLOR_SPACE_HDR10_ST2084_EXT;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkCompareOp ToVkType(Renderer::CompareOp compareOp)
	{
		switch (compareOp)
		{
		case Renderer::CompareOp::Never:
			return VK_COMPARE_OP_NEVER;

		case Renderer::CompareOp::Less:
			return VK_COMPARE_OP_LESS;

		case Renderer::CompareOp::Equal:
			return VK_COMPARE_OP_EQUAL;

		case Renderer::CompareOp::LessOrEqual:
			return VK_COMPARE_OP_LESS_OR_EQUAL;

		case Renderer::CompareOp::Greater:
			return VK_COMPARE_OP_GREATER;

		case Renderer::CompareOp::NotEqual:
			return VK_COMPARE_OP_NOT_EQUAL;

		case Renderer::CompareOp::GreaterOrEqual:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;

		case Renderer::CompareOp::Always:
			return VK_COMPARE_OP_ALWAYS;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	PixelFormat FromVkType(VkFormat format)
	{
		switch (format)
		{
			case VK_FORMAT_R8G8B8A8_UNORM: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8, PixelFormat::Encoding::Normalized);
			case VK_FORMAT_R8G8B8A8_SNORM: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int8, PixelFormat::Encoding::Normalized);
			case VK_FORMAT_R8G8B8A8_USCALED: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8, PixelFormat::Encoding::Scaled);
			case VK_FORMAT_R8G8B8A8_SSCALED: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int8, PixelFormat::Encoding::Scaled);
			case VK_FORMAT_R8G8B8A8_UINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8, PixelFormat::Encoding::Raw);
			case VK_FORMAT_R8G8B8A8_SINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int8, PixelFormat::Encoding::Raw);
			case VK_FORMAT_R8G8B8A8_SRGB: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int8, PixelFormat::Encoding::Srgb);
			case VK_FORMAT_B8G8R8A8_UNORM: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8, PixelFormat::Encoding::Normalized);
			case VK_FORMAT_B8G8R8A8_SNORM: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::Int8, PixelFormat::Encoding::Normalized);
			case VK_FORMAT_B8G8R8A8_USCALED: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8, PixelFormat::Encoding::Scaled);
			case VK_FORMAT_B8G8R8A8_SSCALED: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::Int8, PixelFormat::Encoding::Scaled);
			case VK_FORMAT_B8G8R8A8_UINT: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8, PixelFormat::Encoding::Raw);
			case VK_FORMAT_B8G8R8A8_SINT: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::Int8, PixelFormat::Encoding::Raw);
			case VK_FORMAT_B8G8R8A8_SRGB: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8, PixelFormat::Encoding::Srgb);
			case VK_FORMAT_D16_UNORM: return PixelFormat::DepthStencil(16, 0);
			case VK_FORMAT_X8_D24_UNORM_PACK32: return PixelFormat::DepthStencil(24, 8);
			case VK_FORMAT_D32_SFLOAT: return PixelFormat::DepthStencil(32, 0);
			case VK_FORMAT_S8_UINT: return PixelFormat::DepthStencil(0, 8);
			case VK_FORMAT_D16_UNORM_S8_UINT: return PixelFormat::DepthStencil(16, 8);
			case VK_FORMAT_D24_UNORM_S8_UINT: return PixelFormat::DepthStencil(24, 8);
			case VK_FORMAT_D32_SFLOAT_S8_UINT: return PixelFormat::DepthStencil(32, 8);
			case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt1, false);
			case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt1, true);
			case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::AlphaDxt1, false);
			case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::AlphaDxt1, true);
			case VK_FORMAT_BC2_UNORM_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt3, false);
			case VK_FORMAT_BC2_SRGB_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt3, true);
			case VK_FORMAT_BC3_UNORM_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt5, false);
			case VK_FORMAT_BC3_SRGB_BLOCK: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt5, true);
		}

		return PixelFormat::Undefined();
	}

	VkFormat ToVkType(const PixelFormat& format)
	{
		if (format.IsColor())
		{
			if (format.IsCompressed())
			{
				switch (format.GetCompressionScheme())
				{
				case PixelFormat::CompressionScheme::Dxt1: 
					return format.GetEncoding() == PixelFormat::Encoding::Srgb ? VK_FORMAT_BC1_RGB_SRGB_BLOCK : VK_FORMAT_BC1_RGB_UNORM_BLOCK;

				case PixelFormat::CompressionScheme::AlphaDxt1:
					return format.GetEncoding() == PixelFormat::Encoding::Srgb ? VK_FORMAT_BC1_RGBA_SRGB_BLOCK : VK_FORMAT_BC1_RGBA_UNORM_BLOCK;

				case PixelFormat::CompressionScheme::Dxt3:
					return format.GetEncoding() == PixelFormat::Encoding::Srgb ? VK_FORMAT_BC2_SRGB_BLOCK : VK_FORMAT_BC2_UNORM_BLOCK;

				case PixelFormat::CompressionScheme::Dxt5:
					return format.GetEncoding() == PixelFormat::Encoding::Srgb ? VK_FORMAT_BC3_SRGB_BLOCK : VK_FORMAT_BC2_UNORM_BLOCK;
				}
			}
			else
			{
				switch (format.GetLayout())
				{
				case PixelFormat::Layout::R:
					{
						const PixelFormat::Channel* redChannel = format.GetChannel(0);

						switch (redChannel->Length)
						{
						case 8:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized: 
									return redChannel->IsUnsigned ? VK_FORMAT_R8_UNORM : VK_FORMAT_R8_SNORM;

								case PixelFormat::Encoding::Raw:
									return redChannel->IsUnsigned ? VK_FORMAT_R8_UINT : VK_FORMAT_R8_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R8_USCALED : VK_FORMAT_R8_SSCALED;

								case PixelFormat::Encoding::Srgb:
									return VK_FORMAT_R8_SRGB;
								}
							}
							break;

						case 16:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_R16_UNORM : VK_FORMAT_R16_SNORM;

								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R16_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R16_UINT : VK_FORMAT_R16_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R16_USCALED : VK_FORMAT_R16_SSCALED;
								}
							}
							break;

						case 32:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R32_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R32_UINT : VK_FORMAT_R32_SINT;
								}
							}
							break;

						case 64:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R64_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R64_UINT : VK_FORMAT_R64_SINT;
								}
							}
							break;
						}
					}
					break;

				case PixelFormat::Layout::RG:
					{
						const PixelFormat::Channel* redChannel = format.GetChannel(0);
						const PixelFormat::Channel* greenChannel = format.GetChannel(1);

						switch (redChannel->Length)
						{
						case 8:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8_UNORM : VK_FORMAT_R8G8_SNORM;

								case PixelFormat::Encoding::Raw:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8_UINT : VK_FORMAT_R8G8_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8_USCALED : VK_FORMAT_R8G8_SSCALED;

								case PixelFormat::Encoding::Srgb:
									return VK_FORMAT_R8G8_SRGB;
								}
							}
							break;

							case 16:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_R16G16_UNORM : VK_FORMAT_R16G16_SNORM;

								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R16G16_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R16G16_UINT : VK_FORMAT_R16G16_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R16G16_USCALED : VK_FORMAT_R16G16_SSCALED;
								}
							}
							break;

							case 32:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R32G32_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R32G32_UINT : VK_FORMAT_R32G32_SINT;
								}
							}
							break;

							case 64:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R64G64_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R64G64_UINT : VK_FORMAT_R64G64_SINT;
								}
							}
							break;
						}
					}
					break;

				case PixelFormat::Layout::RGB: 
					{
						const PixelFormat::Channel* redChannel = format.GetChannel(0);
						const PixelFormat::Channel* greenChannel = format.GetChannel(1);
						const PixelFormat::Channel* blueChannel = format.GetChannel(2);

						switch (redChannel->Length)
						{
							case 8:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8B8_UNORM : VK_FORMAT_R8G8B8_SNORM;

								case PixelFormat::Encoding::Raw:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8B8_UINT : VK_FORMAT_R8G8B8_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8B8_USCALED : VK_FORMAT_R8G8B8_SSCALED;

								case PixelFormat::Encoding::Srgb:
									return VK_FORMAT_R8G8B8_SRGB;
								}
							}
							break;

							case 16:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_R16G16B16_UNORM : VK_FORMAT_R16G16B16_SNORM;

								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R16G16B16_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R16G16B16_UINT : VK_FORMAT_R16G16B16_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R16G16B16_USCALED : VK_FORMAT_R16G16B16_SSCALED;
								}
							}
							break;

							case 32:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R32G32B32_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R32G32B32_UINT : VK_FORMAT_R32G32B32_SINT;
								}
							}
							break;

							case 64:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R64G64B64_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R64G64B64_UINT : VK_FORMAT_R64G64B64_SINT;
								}
							}
							break;
						}
					}
					break;

				case PixelFormat::Layout::RGBA:
					{
						const PixelFormat::Channel* redChannel = format.GetChannel(0);

						switch (redChannel->Length)
						{
						case 8:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8B8A8_UNORM : VK_FORMAT_R8G8B8A8_SNORM;

								case PixelFormat::Encoding::Raw:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8B8A8_UINT : VK_FORMAT_R8G8B8A8_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R8G8B8A8_USCALED : VK_FORMAT_R8G8B8A8_SSCALED;

								case PixelFormat::Encoding::Srgb:
									return VK_FORMAT_R8G8B8A8_SRGB;
								}
							}
							break;

						case 16:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_R16G16B16A16_UNORM : VK_FORMAT_R16G16B16A16_SNORM;

								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R16G16B16A16_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R16G16B16A16_UINT : VK_FORMAT_R16G16B16A16_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_R16G16B16A16_USCALED : VK_FORMAT_R16G16B16A16_SSCALED;
								}
							}
							break;

						case 32:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R32G32B32A32_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R32G32B32A32_UINT : VK_FORMAT_R32G32B32A32_SINT;
								}
							}
							break;

						case 64:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Raw:
									if (redChannel->IsFloatingPoint)
										return VK_FORMAT_R64G64B64A64_SFLOAT;

									return redChannel->IsUnsigned ? VK_FORMAT_R64G64B64A64_UINT : VK_FORMAT_R64G64B64A64_SINT;
								}
							}
							break;
						}
					}
					break;

				case PixelFormat::Layout::BGR: break;
				case PixelFormat::Layout::BGRA:
					{
						const PixelFormat::Channel* redChannel = format.GetChannel(0);

						switch (redChannel->Length)
						{
						case 8:
							{
								switch (format.GetEncoding())
								{
								case PixelFormat::Encoding::Normalized:
									return redChannel->IsUnsigned ? VK_FORMAT_B8G8R8A8_UNORM : VK_FORMAT_B8G8R8A8_SNORM;

								case PixelFormat::Encoding::Raw:
									return redChannel->IsUnsigned ? VK_FORMAT_B8G8R8A8_UINT : VK_FORMAT_B8G8R8A8_SINT;

								case PixelFormat::Encoding::Scaled:
									return redChannel->IsUnsigned ? VK_FORMAT_B8G8R8A8_USCALED : VK_FORMAT_B8G8R8A8_SSCALED;

								case PixelFormat::Encoding::Srgb:
									return VK_FORMAT_B8G8R8A8_SRGB;
								}
							}
							break;
						}
					}
					break;
				}
			}
		}
		else if (format.IsDepth())
		{
			const PixelFormat::Channel* depthChannel = format.GetChannel(0);
			switch (depthChannel->Length)
			{
			case 16:
				return VK_FORMAT_D16_UNORM;

			case 24:
				return VK_FORMAT_X8_D24_UNORM_PACK32;

			case 32:
				return VK_FORMAT_D32_SFLOAT;
			}
		}
		else if (format.IsStencil())
		{
			const PixelFormat::Channel* stencilChannel = format.GetChannel(0);
			switch (stencilChannel->Length)
			{
			case 8:
				return VK_FORMAT_S8_UINT;
			}
		}
		else if (format.IsDepthStencil())
		{
			const PixelFormat::Channel* depthChannel = format.GetChannel(0);
			const PixelFormat::Channel* stencilChannel = format.GetChannel(1);
			if (stencilChannel->Length == 8)
			{
				switch (depthChannel->Length)
				{
				case 16:
					return VK_FORMAT_D16_UNORM_S8_UINT;

				case 24:
					return VK_FORMAT_D24_UNORM_S8_UINT;

				case 32:
					return VK_FORMAT_D32_SFLOAT_S8_UINT;
				}
			}
		}

		return VK_FORMAT_UNDEFINED;
	}

	VkPrimitiveTopology ToVkType(Renderer::PrimitiveTopology primitiveTopology)
	{
		switch (primitiveTopology)
		{
		case Renderer::PrimitiveTopology::Point: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case Renderer::PrimitiveTopology::Line:	return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case Renderer::PrimitiveTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case Renderer::PrimitiveTopology::Triangle: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case Renderer::PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case Renderer::PrimitiveTopology::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	Renderer::RasterizationSamples FromVkType(VkSampleCountFlagBits samples)
	{
		switch (samples)
		{
		case VK_SAMPLE_COUNT_1_BIT: return Renderer::RasterizationSamples::e1;
		case VK_SAMPLE_COUNT_2_BIT: return Renderer::RasterizationSamples::e2;
		case VK_SAMPLE_COUNT_4_BIT: return Renderer::RasterizationSamples::e4;
		case VK_SAMPLE_COUNT_8_BIT: return Renderer::RasterizationSamples::e8;
		case VK_SAMPLE_COUNT_16_BIT: return Renderer::RasterizationSamples::e16;
		case VK_SAMPLE_COUNT_32_BIT: return Renderer::RasterizationSamples::e32;
		case VK_SAMPLE_COUNT_64_BIT: return Renderer::RasterizationSamples::e64;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkSampleCountFlagBits ToVkType(Renderer::RasterizationSamples samples)
	{
		switch (samples)
		{
		case Renderer::RasterizationSamples::e1: return VK_SAMPLE_COUNT_1_BIT;
		case Renderer::RasterizationSamples::e2: return VK_SAMPLE_COUNT_2_BIT;
		case Renderer::RasterizationSamples::e4: return VK_SAMPLE_COUNT_4_BIT;
		case Renderer::RasterizationSamples::e8: return VK_SAMPLE_COUNT_8_BIT;
		case Renderer::RasterizationSamples::e16: return VK_SAMPLE_COUNT_16_BIT;
		case Renderer::RasterizationSamples::e32: return VK_SAMPLE_COUNT_32_BIT;
		case Renderer::RasterizationSamples::e64: return VK_SAMPLE_COUNT_64_BIT;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkOffset2D ToVkType(const Extent2D<int>& extent)
	{
		return {
			extent.Width,
			extent.Height
		};
	}

	VkOffset3D ToVkType(const Extent3D<int>& extent)
	{
		return {
			extent.Width,
			extent.Height
		};
	}

	VkExtent2D ToVkType(const Extent2D<unsigned int>& extent)
	{
		return {
			extent.Width,
			extent.Height
		};
	}

	VkExtent3D ToVkType(const Extent3D<unsigned int>& extent)
	{
		return {
			extent.Width,
			extent.Height,
			extent.Depth
		};
	}

	VkImageType ToVkType(Renderer::TextureType type)
	{
		switch (type)
		{
		case Renderer::TextureType::e1D: return VK_IMAGE_TYPE_1D;
		case Renderer::TextureType::e2D: return VK_IMAGE_TYPE_2D;
		case Renderer::TextureType::e3D: return VK_IMAGE_TYPE_3D;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkBufferUsageFlags ToVkType(Renderer::BufferUsageFlagBits usage)
	{
		switch (usage)
		{
		case Renderer::BufferUsageFlagBits::Vertex: return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		case Renderer::BufferUsageFlagBits::Index: return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		case Renderer::BufferUsageFlagBits::Uniform: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		case Renderer::BufferUsageFlagBits::Storage: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		case Renderer::BufferUsageFlagBits::Indirect: return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkBufferUsageFlags ToVkTypes(const Renderer::BufferUsageFlags& usage)
	{
		VkBufferUsageFlags usageFlags = 0;
		for (auto value : Enum<Renderer::BufferUsageFlagBits>::Values)
		{
			if (usage & value)
				usageFlags |= ToVkType(value);
		}

		return usageFlags;
	}

	VkShaderStageFlagBits ToVkType(Renderer::ShaderType type)
	{
		switch (type)
		{
		case Renderer::ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		case Renderer::ShaderType::TessellationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case Renderer::ShaderType::TessellationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case Renderer::ShaderType::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		case Renderer::ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case Renderer::ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkShaderStageFlags ToVkTypes(const Flags<Renderer::ShaderType>& types)
	{
		VkShaderStageFlags shaderStageFlags = 0;
		for (auto value : Enum<Renderer::ShaderType>::Values)
		{
			if (types & value)
				shaderStageFlags |= ToVkType(value);
		}

		return shaderStageFlags;
	}

	VkViewport ToVkType(const Renderer::Viewport& viewport)
	{
		return {
			viewport.X,
			viewport.Y,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth
		};
	}

	VkRect2D ToVkType(const Renderer::Scissor& scissor)
	{
		return {
			ToVkType(scissor.Position),
			ToVkType(scissor.Size)
		};
	}

	VkBlendFactor ToVkType(Renderer::BlendFactor blendFactor)
	{
		switch (blendFactor)
		{
		case Renderer::BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
		case Renderer::BlendFactor::One: return VK_BLEND_FACTOR_ONE;
		case Renderer::BlendFactor::SourceColor: return VK_BLEND_FACTOR_SRC_COLOR;
		case Renderer::BlendFactor::OneMinusSourceColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case Renderer::BlendFactor::DestinationColor: return VK_BLEND_FACTOR_DST_COLOR;
		case Renderer::BlendFactor::OneMinusDestinationColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case Renderer::BlendFactor::SourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
		case Renderer::BlendFactor::OneMinusSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case Renderer::BlendFactor::DestinationAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
		case Renderer::BlendFactor::OneMinusDestinationAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case Renderer::BlendFactor::ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
		case Renderer::BlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
		case Renderer::BlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
		case Renderer::BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		case Renderer::BlendFactor::SourceAlphaSaturate: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkBlendOp ToVkType(Renderer::BlendOp blendOp)
	{
		switch (blendOp)
		{
		case Renderer::BlendOp::Add: return VK_BLEND_OP_ADD;
		case Renderer::BlendOp::Subtract: return VK_BLEND_OP_SUBTRACT;
		case Renderer::BlendOp::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
		case Renderer::BlendOp::Min: return VK_BLEND_OP_MIN;
		case Renderer::BlendOp::Max: return VK_BLEND_OP_MAX;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkPolygonMode ToVkType(Renderer::PolygonMode polygonMode)
	{
		switch (polygonMode)
		{
		case Renderer::PolygonMode::Fill: return VK_POLYGON_MODE_FILL;
		case Renderer::PolygonMode::Line: return VK_POLYGON_MODE_LINE;
		case Renderer::PolygonMode::Point: return VK_POLYGON_MODE_POINT;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkCullModeFlagBits ToVkType(Renderer::CullMode cullMode)
	{
		switch (cullMode)
		{
		case Renderer::CullMode::None:  return VK_CULL_MODE_NONE;
		case Renderer::CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
		case Renderer::CullMode::Back:  return VK_CULL_MODE_BACK_BIT;
		case Renderer::CullMode::Both:  return VK_CULL_MODE_FRONT_AND_BACK;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkFrontFace ToVkType(Renderer::FrontFace frontFace)
	{
		switch (frontFace)
		{
		case Renderer::FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
		case Renderer::FrontFace::Clockwise: return VK_FRONT_FACE_CLOCKWISE;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkLogicOp ToVkType(Renderer::LogicOp logicOp)
	{
		switch (logicOp)
		{
		case Renderer::LogicOp::OpClear: return VK_LOGIC_OP_CLEAR;
		case Renderer::LogicOp::OpAnd: return VK_LOGIC_OP_AND;
		case Renderer::LogicOp::OpAndReverse: return VK_LOGIC_OP_AND_REVERSE;
		case Renderer::LogicOp::OpCopy: return VK_LOGIC_OP_COPY;
		case Renderer::LogicOp::OpAndInverted: return VK_LOGIC_OP_AND_INVERTED;
		case Renderer::LogicOp::OpNoOp: return VK_LOGIC_OP_NO_OP;
		case Renderer::LogicOp::OpExclusiveOr: return VK_LOGIC_OP_XOR;
		case Renderer::LogicOp::OpOr: return VK_LOGIC_OP_OR;
		case Renderer::LogicOp::OpNotOr: return VK_LOGIC_OP_NOR;
		case Renderer::LogicOp::OpEquivalent: return VK_LOGIC_OP_EQUIVALENT;
		case Renderer::LogicOp::OpInvert: return VK_LOGIC_OP_INVERT;
		case Renderer::LogicOp::OpOrReverse: return VK_LOGIC_OP_OR_REVERSE;
		case Renderer::LogicOp::OpCopyInverted: return VK_LOGIC_OP_COPY_INVERTED;
		case Renderer::LogicOp::OpOrInverted: return VK_LOGIC_OP_OR_INVERTED;
		case Renderer::LogicOp::OpNotAnd: return VK_LOGIC_OP_NAND;
		case Renderer::LogicOp::OpSet: return VK_LOGIC_OP_SET;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkIndexType ToVkType(Renderer::IndexType indexType)
	{
		switch (indexType)
		{
		case Renderer::IndexType::Short:
			return VK_INDEX_TYPE_UINT16;

		case Renderer::IndexType::Integer:
			return VK_INDEX_TYPE_UINT32;

		case Renderer::IndexType::Byte:
			return VK_INDEX_TYPE_UINT8_EXT;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkDescriptorType ToVkType(Renderer::DescriptorType descriptorType)
	{
		switch (descriptorType)
		{
		case Renderer::DescriptorType::Sampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		case Renderer::DescriptorType::TextureSampler:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		case Renderer::DescriptorType::Texture:
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		case Renderer::DescriptorType::StorageTexture:
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

		case Renderer::DescriptorType::UniformBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		case Renderer::DescriptorType::StorageBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkFilter ToVkType(Renderer::Filter filter)
	{
		switch (filter)
		{
		case Renderer::Filter::Nearest:
			return VK_FILTER_NEAREST;

		case Renderer::Filter::Linear:
			return VK_FILTER_LINEAR;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkSamplerMipmapMode ToVkType(Renderer::SamplerMipmapMode mipmapMode)
	{
		switch (mipmapMode)
		{
		case Renderer::SamplerMipmapMode::Nearest:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;

		case Renderer::SamplerMipmapMode::Linear:
			return VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkSamplerAddressMode ToVkType(Renderer::SamplerAddressMode addressMode)
	{
		switch (addressMode)
		{
		case Renderer::SamplerAddressMode::Repeat:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;

		case Renderer::SamplerAddressMode::MirroredRepeat:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

		case Renderer::SamplerAddressMode::ClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		case Renderer::SamplerAddressMode::ClampToBorder:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkDynamicState ToVkType(Renderer::CommandListDynamicStateBits dynamicState)
	{
		switch (dynamicState)
		{
		case Renderer::CommandListDynamicStateBits::Viewport: 
			return VK_DYNAMIC_STATE_VIEWPORT;

		case Renderer::CommandListDynamicStateBits::Scissor:
			return VK_DYNAMIC_STATE_SCISSOR;

		case Renderer::CommandListDynamicStateBits::LineWidth:
			return VK_DYNAMIC_STATE_LINE_WIDTH;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	VkResolveModeFlagBitsKHR ToVkType(Renderer::ResolveMode resolveMode)
	{
		switch (resolveMode)
		{
		case Renderer::ResolveMode::None:
			return VK_RESOLVE_MODE_NONE_KHR;

		case Renderer::ResolveMode::SampleZero:
			return VK_RESOLVE_MODE_SAMPLE_ZERO_BIT_KHR;

		case Renderer::ResolveMode::Average:
			return VK_RESOLVE_MODE_AVERAGE_BIT_KHR;

		case Renderer::ResolveMode::Min:
			return VK_RESOLVE_MODE_MIN_BIT_KHR;

		case Renderer::ResolveMode::Max:
			return VK_RESOLVE_MODE_MAX_BIT_KHR;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}
}
