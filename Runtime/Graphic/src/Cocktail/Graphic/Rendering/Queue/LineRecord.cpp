#include <Cocktail/Graphic/Rendering/Queue/LineRecord.hpp>

namespace Ck
{
    ObjectPool<LineRecord> LineRecord::sRecordPool;

    std::shared_ptr<LineRecord> LineRecord::New(const LineRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant)
    {
        return sRecordPool.Allocate(recordInfo, materialProgramVariant);
    }

    LineRecord::LineRecord(const LineRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant) :
        mRecordInfo(recordInfo),
        mMaterialProgramVariant(materialProgramVariant)
    {
        /// Nothing
    }

    void LineRecord::Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const
    {
        drawContext.BindMaterialProgram(commandList, *mMaterialProgramVariant);

        VertexInfo vertexInfo;
        vertexInfo.Model = mRecordInfo.WorldMatrix.Transpose();
        commandList.UpdatePipelineConstant(Renderer::ShaderType::Vertex, 0, sizeof(VertexInfo), &vertexInfo);

        FragmentInfo fragmentInfo;
        fragmentInfo.Color = mRecordInfo.Color;
        commandList.UpdatePipelineConstant(Renderer::ShaderType::Fragment, 0, sizeof(FragmentInfo), &fragmentInfo);

        Renderer::VertexAttributeLocation* positionAttributeLocation = mMaterialProgramVariant->GetVertexAttributeLocation(VertexAttributeSemantic::Position);
        Renderer::VertexAttributeLocation* colorAttributeLocation = mMaterialProgramVariant->GetVertexAttributeLocation(VertexAttributeSemantic::Color);
        assert(positionAttributeLocation);

        const unsigned int PositionAttributeStride = positionAttributeLocation->GetElementCount() * positionAttributeLocation->GetDataType().GetSize();

        if (colorAttributeLocation)
        {
            const unsigned int ColorAttributeStride = colorAttributeLocation->GetElementCount() * colorAttributeLocation->GetDataType().GetSize();

            commandList.EnableVertexBinding(0, true);
            commandList.BindVertexBuffer(0, mRecordInfo.VertexBuffer, 0, PositionAttributeStride + ColorAttributeStride);

            Renderer::VertexInputAttribute inputAttributes[] = {
                { positionAttributeLocation->GetLocation(), PixelFormat::Color(PixelFormat::Layout::RGB, DataType::Float32, PixelFormat::Encoding::Raw), 0 },
                { colorAttributeLocation->GetLocation(), PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Float32, PixelFormat::Encoding::Raw), PositionAttributeStride }
            };
            commandList.SetVertexInputAttributes(0, 2, inputAttributes);
        }
        else
        {
            commandList.EnableVertexBinding(0, true);
            commandList.BindVertexBuffer(0, mRecordInfo.VertexBuffer, 0, PositionAttributeStride);

            Renderer::VertexInputAttribute inputAttributes[] = {
                { positionAttributeLocation->GetLocation(), PixelFormat::Color(PixelFormat::Layout::RGB, DataType::Float32, PixelFormat::Encoding::Raw), 0 }
            };
            commandList.SetVertexInputAttributes(0, 1, inputAttributes);
        }

        commandList.SetPrimitiveTopology(mRecordInfo.IsStrip ? Renderer::PrimitiveTopology::LineStrip : Renderer::PrimitiveTopology::Line);

        commandList.SetPolygonMode(Renderer::PolygonMode::Line);
        commandList.SetLineWidth(mRecordInfo.Thickness);

        commandList.EnableDepthTest(true);
        commandList.EnableDepthWrite(mRecordInfo.Color.IsOpaque());
        commandList.SetDepthCompareOp(Renderer::CompareOp::Less);

        commandList.EnableBlending(0, mRecordInfo.Color.IsTransparent());
        commandList.SetBlendingFunction(0, Renderer::BlendFactor::SourceAlpha, Renderer::BlendFactor::OneMinusSourceAlpha, Renderer::BlendFactor::One, Renderer::BlendFactor::OneMinusSourceAlpha);
        commandList.SetBlendingEquation(0, Renderer::BlendOp::Add, Renderer::BlendOp::Add);

        commandList.Draw(mRecordInfo.PointCount, 1);
    }
}
