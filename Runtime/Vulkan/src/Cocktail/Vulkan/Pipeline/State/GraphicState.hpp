#ifndef COCKTAIL_VULKAN_PIPELINE_GRAPHICSTATE_HPP
#define COCKTAIL_VULKAN_PIPELINE_GRAPHICSTATE_HPP

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Renderer.hpp>

#include <Cocktail/Vulkan/Pipeline/State/ShaderStageState.hpp>

namespace Ck::Vulkan
{
	constexpr unsigned int MaxVertexAttributes = 64;
	constexpr unsigned int MaxInputBindings = 32;
	constexpr unsigned int MaxViewports = 16;
	constexpr unsigned int MaxColorAttachments = 8;

	struct VertexInputBinding
	{
		bool Enable = false;
		unsigned int AttributeCount = 0;
		Renderer::VertexInputAttribute Attributes[MaxVertexAttributes] = {};
		unsigned int Stride = 0;
		bool Instanced = false;
		unsigned int Divisor = 0;
	};

	struct VertexInputState
	{
		VertexInputBinding Bindings[MaxInputBindings] = {};
	};

	struct InputAssemblyState
	{
		Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
		bool PrimitiveRestartEnable = false;
	};

	struct ViewportState
	{
		unsigned int ViewportCount = 1;
		Renderer::Viewport Viewports[MaxViewports] = {};
		unsigned int ScissorCount = 1;
		Renderer::Scissor Scissors[MaxViewports] = {};
	};

	struct RasterizationState
	{
		bool RasterizerDiscardEnable = false;
		Renderer::PolygonMode PolygonMode = Renderer::PolygonMode::Fill;
		Renderer::CullMode CullMode = Renderer::CullMode::None;
		Renderer::FrontFace FrontFace = Renderer::FrontFace::CounterClockwise;
		bool DepthBiasEnable = false;
		float DepthBiasConstantFactor = 0.f;
		float DepthBiasClamp = 0.f;
		float DepthBiasSlopeFactor = 0.f;
		float LineWidth = 1.f;
	};

	struct MultisampleState
	{
		Renderer::RasterizationSamples Samples = Renderer::RasterizationSamples::e1;
		bool SampleShadingEnable = false;
		float MinSampleShading = 0.f;
		bool AlphaToCoverageEnable = false;
		bool AlphaToOneEnable = false;
	};

	struct DepthStencilState
	{
		bool DepthTestEnable = false;
		bool DepthWriteEnable = false;
		Renderer::CompareOp DepthCompareOp = Renderer::CompareOp::Less;
		bool DepthBoundsTestEnable = false;
		float MinDepthBounds = 0.f;
		float MaxDepthBounds = 1.f;
	};

	struct ColorBlendAttachmentState
	{
		bool BlendEnable = false;
		Renderer::BlendFactor SourceColorBlendFactor = Renderer::BlendFactor::SourceAlpha;
		Renderer::BlendFactor DestinationColorBlendFactor = Renderer::BlendFactor::OneMinusSourceAlpha;
		Renderer::BlendOp ColorBlendOp = Renderer::BlendOp::Add;
		Renderer::BlendFactor SourceAlphaBlendFactor = Renderer::BlendFactor::One;
		Renderer::BlendFactor DestinationAlphaBlendFactor = Renderer::BlendFactor::OneMinusSourceAlpha;
		Renderer::BlendOp AlphaBlendOp = Renderer::BlendOp::Add;
		bool ColorWriteMask[4] = { true, true, true, true };
	};

	struct ColorBlendState
	{
		bool LogicOpEnable = false;
		Renderer::LogicOp LogicOp = Renderer::LogicOp::OpClear;
		unsigned int ColorBlendAttachmentCount = 0;
		ColorBlendAttachmentState ColorBlendAttachments[MaxColorAttachments] = {};
		LinearColor BlendConstants = { 0.f, 0.f, 0.f, 0.f };
	};

	struct GraphicState
	{
		EnumMap<Renderer::ShaderType, ShaderStageState> ShaderStages;
		VertexInputState VertexInput;
		InputAssemblyState InputAssembly;
		ViewportState Viewport;
		RasterizationState Rasterization;
		MultisampleState Multisample;
		DepthStencilState DepthStencil;
		ColorBlendState ColorBlend;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_GRAPHICSTATE_HPP
