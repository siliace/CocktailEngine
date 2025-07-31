#include <Cocktail/Core/Math/Matrix/Matrix3.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
	ObjectPool<StaticMeshRecord> StaticMeshRecord::sRecordPool;

	bool IsStripTopology(Renderer::PrimitiveTopology primitiveTopology)
	{
		switch (primitiveTopology)
		{
		case Renderer::PrimitiveTopology::LineStrip:
		case Renderer::PrimitiveTopology::TriangleStrip:
			return true;
		}

		return false;
	}

	Renderer::PolygonMode GetPrimitivePolygonMode(Renderer::PrimitiveTopology primitiveTopology)
	{
		switch (primitiveTopology)
		{
		case Renderer::PrimitiveTopology::Point:
			return Renderer::PolygonMode::Point;

		case Renderer::PrimitiveTopology::Line:
		case Renderer::PrimitiveTopology::LineStrip:
			return Renderer::PolygonMode::Line;

		case Renderer::PrimitiveTopology::Triangle:
		case Renderer::PrimitiveTopology::TriangleStrip:
		case Renderer::PrimitiveTopology::TriangleFan:
			return Renderer::PolygonMode::Fill;
		}

		COCKTAIL_UNREACHABLE();
	}

	std::shared_ptr<StaticMeshRecord> StaticMeshRecord::New(const StaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant)
	{
		return sRecordPool.Allocate(recordInfo, materialProgramVariant);
	}

	StaticMeshRecord::StaticMeshRecord(const StaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant) :
		mRecordInfo(recordInfo),
		mMaterialProgramVariant(materialProgramVariant)
	{
		/// Nothing
	}

	void StaticMeshRecord::Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const
	{
		drawContext.BindMaterialProgram(commandList, *mMaterialProgramVariant);

		VertexInfo vertexInfo;
		vertexInfo.Model = mRecordInfo.Model.Transpose();
		Matrix3<float> normalMatrix = Matrix3<float>::From(mRecordInfo.Model).Inverse().Transpose();
		for (unsigned int i = 0; i < 3; i++)
			vertexInfo.Normal[i] = Vector4<float>(normalMatrix.GetColumn(i).Normalized(), 0.f);

		MaterialInfo materialInfo;
		materialInfo.BaseColor = mRecordInfo.MaterialBaseColor;
		materialInfo.SpecularColor[0] = mRecordInfo.MaterialSpecularColor.R;
		materialInfo.SpecularColor[1] = mRecordInfo.MaterialSpecularColor.G;
		materialInfo.SpecularColor[2] = mRecordInfo.MaterialSpecularColor.B;
		materialInfo.AlphaMode = static_cast<int>(mRecordInfo.AlphaMode);
		materialInfo.EmissiveColor[0] = mRecordInfo.MaterialEmissiveColor.R;
		materialInfo.EmissiveColor[1] = mRecordInfo.MaterialEmissiveColor.G;
		materialInfo.EmissiveColor[2] = mRecordInfo.MaterialEmissiveColor.B;
		materialInfo.AlphaCutoff = mRecordInfo.AlphaCutoff;

		commandList.UpdatePipelineConstant(Renderer::ShaderType::Vertex, 0, sizeof(VertexInfo), &vertexInfo);
		commandList.UpdatePipelineConstant(Renderer::ShaderType::Fragment, 0, sizeof(MaterialInfo), &materialInfo);
		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			if (Renderer::UniformSlot* slot = mMaterialProgramVariant->GetMaterialTextureSlot(textureType))
			{
				std::shared_ptr<Renderer::TextureView> textureView = mRecordInfo.MaterialTextures[textureType];
				if (!textureView)
					continue;

				commandList.BindTextureSampler(slot, 0, textureView.get(), nullptr);
			}
		}

		for (unsigned int i = 0; i < mRecordInfo.VertexBufferCount; i++)
		{
			const StaticMeshRecordInfo::VertexBuffer& vertexBuffer = mRecordInfo.VertexBuffers[i];

			const VertexLayout* vertexLayout = vertexBuffer.VertexLayout;

			commandList.EnableVertexBinding(i, true);
			commandList.BindVertexBuffer(i, vertexBuffer.Buffer, vertexBuffer.Offset, vertexLayout->GetStride());
			drawContext.SetVertexInputAttributes(commandList, i, *vertexLayout);
		}

		if (drawContext.GetModifiers() & RecordDrawContext::RenderingModifierBits::Wireframe)
		{
			commandList.SetPrimitiveTopology(IsStripTopology(mRecordInfo.PrimitiveTopology) ? Renderer::PrimitiveTopology::LineStrip : Renderer::PrimitiveTopology::Line);
			commandList.SetPolygonMode(Renderer::PolygonMode::Line);
		}
		else
		{
			commandList.SetPrimitiveTopology(mRecordInfo.PrimitiveTopology);
			commandList.SetPolygonMode(GetPrimitivePolygonMode(mRecordInfo.PrimitiveTopology));
		}

		commandList.SetCullMode(mRecordInfo.DoubleSided ? Renderer::CullMode::None : Renderer::CullMode::Back);
		commandList.SetFrontFace(Renderer::FrontFace::CounterClockwise);

		commandList.EnableDepthTest(true);
		commandList.EnableDepthWrite(mRecordInfo.AlphaMode == Material::AlphaMode::Opaque);
		commandList.SetDepthCompareOp(Renderer::CompareOp::Less);

		commandList.EnableBlending(0, mRecordInfo.AlphaMode != Material::AlphaMode::Opaque);
		commandList.SetBlendingFunction(0, Renderer::BlendFactor::SourceAlpha, Renderer::BlendFactor::OneMinusSourceAlpha, Renderer::BlendFactor::One, Renderer::BlendFactor::OneMinusSourceAlpha);
		commandList.SetBlendingEquation(0, Renderer::BlendOp::Add, Renderer::BlendOp::Add);

		if (mRecordInfo.IndexBuffer)
		{
			commandList.BindIndexBuffer(mRecordInfo.IndexBuffer, mRecordInfo.IndexBufferOffset, mRecordInfo.IndexType);
			commandList.DrawIndexed(mRecordInfo.Count, 1, mRecordInfo.FirstIndex, mRecordInfo.FirstVertex, 0);
		}
		else
		{
			commandList.Draw(mRecordInfo.Count, 1, mRecordInfo.FirstIndex, 0);
		}

		for (unsigned int i = 0; i < mRecordInfo.VertexBufferCount; i++)
			commandList.EnableVertexBinding(i, false);
	}
}
