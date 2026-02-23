#include <Cocktail/Core/Math/Matrix/Matrix3.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Graphic/Rendering/Queue/InstancedStaticMeshRecord.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
    namespace
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
    }

	ObjectPool<InstancedStaticMeshRecord> InstancedStaticMeshRecord::sRecordPool;

	std::shared_ptr<InstancedStaticMeshRecord> InstancedStaticMeshRecord::New(const InstancedStaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant)
	{
		return sRecordPool.Allocate(recordInfo, materialProgramVariant);
	}

	InstancedStaticMeshRecord::InstancedStaticMeshRecord(const InstancedStaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant) :
		mRecordInfo(recordInfo),
		mMaterialProgramVariant(materialProgramVariant)
	{
		/// Nothing
	}

	void InstancedStaticMeshRecord::Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const
	{
	    const StaticMeshRecordInfo& staticMeshRecordInfo = mRecordInfo.StaticMeshRecord;

		drawContext.BindMaterialProgram(commandList, *mMaterialProgramVariant);

		VertexInfo vertexInfo;
		vertexInfo.Model = staticMeshRecordInfo.Model.Transpose();
		Matrix3<float> normalMatrix = Matrix3<float>::From(staticMeshRecordInfo.Model).Inverse().Transpose();
		for (unsigned int i = 0; i < 3; i++)
			vertexInfo.Normal[i] = Vector4<float>(normalMatrix.GetColumn(i).Normalized(), 0.f);

		MaterialInfo materialInfo;
	    materialInfo.BaseColor = staticMeshRecordInfo.MaterialBaseColor;
	    materialInfo.EmissiveColor[0] = staticMeshRecordInfo.MaterialEmissiveColor.R;
	    materialInfo.EmissiveColor[1] = staticMeshRecordInfo.MaterialEmissiveColor.G;
	    materialInfo.EmissiveColor[2] = staticMeshRecordInfo.MaterialEmissiveColor.B;
	    materialInfo.Roughness = staticMeshRecordInfo.MaterialRoughness;
	    materialInfo.Metallic = staticMeshRecordInfo.MaterialMetallic;
		materialInfo.AlphaMode = static_cast<int>(staticMeshRecordInfo.AlphaMode);
		materialInfo.AlphaCutoff = staticMeshRecordInfo.AlphaCutoff;

		commandList.UpdatePipelineConstant(Renderer::ShaderType::Vertex, 0, sizeof(VertexInfo), &vertexInfo);
		commandList.UpdatePipelineConstant(Renderer::ShaderType::Fragment, 0, sizeof(MaterialInfo), &materialInfo);
		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			if (Renderer::UniformSlot* slot = mMaterialProgramVariant->GetMaterialTextureSlot(textureType))
			{
				std::shared_ptr<Renderer::TextureView> textureView = staticMeshRecordInfo.MaterialTextures[textureType];
				if (!textureView)
					continue;

				commandList.BindTextureSampler(slot, 0, textureView.get(), nullptr);
			}
		}

	    drawContext.BindBuffer(commandList, "instances", mRecordInfo.InstancesBuffer, mRecordInfo.InstanceBufferOffset);

		for (unsigned int i = 0; i < staticMeshRecordInfo.VertexBufferCount; i++)
		{
			const StaticMeshRecordInfo::VertexBuffer& vertexBuffer = staticMeshRecordInfo.VertexBuffers[i];

			const VertexLayout* vertexLayout = vertexBuffer.VertexLayout;

			commandList.EnableVertexBinding(i, true);
			commandList.BindVertexBuffer(i, vertexBuffer.Buffer, vertexBuffer.Offset, vertexLayout->GetStride());
			drawContext.SetVertexInputAttributes(commandList, i, *vertexLayout);
		}

		if (drawContext.GetModifiers() & RecordDrawContext::RenderingModifierBits::Wireframe)
		{
			commandList.SetPrimitiveTopology(IsStripTopology(staticMeshRecordInfo.PrimitiveTopology) ? Renderer::PrimitiveTopology::LineStrip : Renderer::PrimitiveTopology::Line);
			commandList.SetPolygonMode(Renderer::PolygonMode::Line);
		}
		else
		{
			commandList.SetPrimitiveTopology(staticMeshRecordInfo.PrimitiveTopology);
			commandList.SetPolygonMode(GetPrimitivePolygonMode(staticMeshRecordInfo.PrimitiveTopology));
		}

		commandList.SetCullMode(staticMeshRecordInfo.DoubleSided ? Renderer::CullMode::None : Renderer::CullMode::Back);
		commandList.SetFrontFace(Renderer::FrontFace::CounterClockwise);

		commandList.EnableDepthTest(true);
		commandList.EnableDepthWrite(staticMeshRecordInfo.AlphaMode == Material::AlphaMode::Opaque);
		commandList.SetDepthCompareOp(Renderer::CompareOp::Less);

		commandList.EnableBlending(0, staticMeshRecordInfo.AlphaMode != Material::AlphaMode::Opaque);
		commandList.SetBlendingFunction(0, Renderer::BlendFactor::SourceAlpha, Renderer::BlendFactor::OneMinusSourceAlpha, Renderer::BlendFactor::One, Renderer::BlendFactor::OneMinusSourceAlpha);
		commandList.SetBlendingEquation(0, Renderer::BlendOp::Add, Renderer::BlendOp::Add);

		if (staticMeshRecordInfo.IndexBuffer)
		{
			commandList.BindIndexBuffer(staticMeshRecordInfo.IndexBuffer, staticMeshRecordInfo.IndexBufferOffset, staticMeshRecordInfo.IndexType);
			commandList.DrawIndexed(staticMeshRecordInfo.Count, mRecordInfo.InstanceCount, staticMeshRecordInfo.FirstIndex, staticMeshRecordInfo.FirstVertex, mRecordInfo.FirstInstance);
		}
		else
		{
			commandList.Draw(staticMeshRecordInfo.Count,  mRecordInfo.InstanceCount, staticMeshRecordInfo.FirstIndex, mRecordInfo.FirstInstance);
		}

		for (unsigned int i = 0; i < staticMeshRecordInfo.VertexBufferCount; i++)
			commandList.EnableVertexBinding(i, false);
	}
}
