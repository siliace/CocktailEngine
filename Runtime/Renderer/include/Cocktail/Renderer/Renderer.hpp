#ifndef COCKTAIL_RENDERER_RENDERER_HPP
#define COCKTAIL_RENDERER_RENDERER_HPP

#include <Cocktail/Renderer/Framebuffer/FramebufferLayout.hpp>

namespace Ck::Renderer
{
	static constexpr unsigned int MaxVertexInputBindingCount = 8;
	static constexpr unsigned int MaxVertexInputAttributeCount = 32;

	struct VertexInputAttribute
	{
		unsigned int Location = 0;
		PixelFormat Format;
		unsigned int Offset = 0;
	};

	enum class PrimitiveTopology
	{
		Point,
		Line,
		LineStrip,
		Triangle,
		TriangleStrip,
		TriangleFan,
	};

	struct Viewport
	{
		float X = 0.f;
		float Y = 0.f;
		float Width = 0.f;
		float Height = 0.f;
		float MinDepth = 0.f;
		float MaxDepth = 0.f;

		friend bool operator==(const Viewport& lhs, const Viewport& rhs)
		{
			return std::tie(lhs.X, lhs.Y, lhs.Width, lhs.Height, lhs.MinDepth, lhs.MaxDepth) == std::tie(rhs.X, rhs.Y, rhs.Width, rhs.Height, rhs.MinDepth, rhs.MaxDepth);
		}

		friend bool operator!=(const Viewport& lhs, const Viewport& rhs)
		{
			return !(lhs == rhs);
		}
	};

	struct Scissor
	{
		Extent2D<int> Position;
		Extent2D<unsigned int> Size;

		friend bool operator==(const Scissor& lhs, const Scissor& rhs)
		{
			return std::tie(lhs.Position, lhs.Size) == std::tie(rhs.Position, rhs.Size);
		}

		friend bool operator!=(const Scissor& lhs, const Scissor& rhs)
		{
			return !(lhs == rhs);
		}
	};

	enum class PolygonMode
	{
		Fill,
		Line,
		Point,
	};

	enum class CullMode
	{
		None,
		Front,
		Back,
		Both,
	};

	enum class FrontFace
	{
		CounterClockwise,
		Clockwise
	};

	enum class CompareOp
	{
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always
	};

	struct StencilOpState
	{
		// StencilOp FailOp;
		// StencilOp PassOp;
		// StencilOp DepthFailOp;
		CompareOp CompareOp;
		unsigned int CompareMask;
		unsigned int WriteMask;
		unsigned int Reference;
	};

	enum class LogicOp
	{
		OpClear,
		OpAnd,
		OpAndReverse,
		OpCopy,
		OpAndInverted,
		OpNoOp,
		OpExclusiveOr,
		OpOr,
		OpNotOr,
		OpEquivalent,
		OpInvert,
		OpOrReverse,
		OpCopyInverted,
		OpOrInverted,
		OpNotAnd,
		OpSet,
	};

	enum class BlendOp
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max,
	};

	enum class BlendFactor
	{
		Zero,
		One,
		SourceColor,
		OneMinusSourceColor,
		DestinationColor,
		OneMinusDestinationColor,
		SourceAlpha,
		OneMinusSourceAlpha,
		DestinationAlpha,
		OneMinusDestinationAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SourceAlphaSaturate
	};
}

#endif // COCKTAIL_RENDERER_RENDERER_HPP
