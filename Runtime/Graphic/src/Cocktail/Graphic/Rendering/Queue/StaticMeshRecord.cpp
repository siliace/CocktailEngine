#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
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

	Ref<StaticMeshRecord> StaticMeshRecord::New(const StaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant)
	{
		static ObjectPool<StaticMeshRecord> RecordPool;
		return RecordPool.Allocate(recordInfo, materialProgramVariant);
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

		Matrix4<float> modelMatrix = mRecordInfo.Model;
		Matrix3<float> normalMatrix(modelMatrix.GetRow(0).XYZ(), modelMatrix.GetRow(1).XYZ(), modelMatrix.GetRow(2).XYZ());

		VertexInfo vertexInfo;
		vertexInfo.Model = modelMatrix.Transpose();
		vertexInfo.Normal = normalMatrix.Inverse().Transpose();

		drawContext.BindPipelineConstantsData(commandList, Renderer::ShaderType::Vertex, 0, sizeof(VertexInfo), &vertexInfo);
		drawContext.BindPipelineConstantsData(commandList, Renderer::ShaderType::Fragment, 0, sizeof(Material::UniformColors), &mRecordInfo.UniformColors);
		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			if (Renderer::UniformSlot* slot = mMaterialProgramVariant->GetMaterialTextureSlot(textureType))
			{
				Ref<Renderer::TextureView> textureView = mRecordInfo.MaterialTextures[textureType];
				if (!textureView)
					continue;

				static Ref<Renderer::Sampler> sampler = commandList.GetRenderDevice()->CreateSampler({});
				assert(sampler);

				commandList.BindTextureSampler(slot, 0, textureView.Get(), sampler.Get());
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
		commandList.EnableDepthWrite(true);
		commandList.SetDepthCompareOp(Renderer::CompareOp::Less);

		commandList.EnableBlending(0, !mRecordInfo.Opaque);
		commandList.SetBlendingFunction(0, Renderer::BlendFactor::SourceAlpha, Renderer::BlendFactor::OneMinusSourceAlpha, Renderer::BlendFactor::One, Renderer::BlendFactor::OneMinusSourceAlpha);
		commandList.SetBlendingEquation(0, Renderer::BlendOp::Add, Renderer::BlendOp::Add);

		if (mRecordInfo.IndexBuffer)
		{
			commandList.BindIndexBuffer(mRecordInfo.IndexBuffer, mRecordInfo.IndexBufferOffset, mRecordInfo.IndexType);
			commandList.DrawIndexed(mRecordInfo.Count, 1, 0, 0, 0);
		}
		else
		{
			commandList.Draw(mRecordInfo.Count, 1, 0, 0);
		}

		for (unsigned int i = 0; i < mRecordInfo.VertexBufferCount; i++)
			commandList.EnableVertexBinding(i, false);
	}
}