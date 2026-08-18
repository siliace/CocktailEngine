#include <CocktailEngine/Graphic/Scene/Shape/LineShape.hpp>

namespace Ck
{
    LineShape::LineShape(GraphicEngine& graphicEngine, Array<Vector3<float>> points) :
        mPointCount(points.GetSize())
    {
        VertexLayout::Builder builder;
        builder.AddAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3);

        SharedPtr<VertexArray> vertexArray = MakeShared<VertexArray>(builder.Get(), mPointCount);
        for (std::size_t i = 0; i < vertexArray->GetVertexCount(); i++)
        {
            VertexRef vertex = vertexArray->At(i);
            vertex.SetVector(VertexAttributeSemantic::Position, points[i]);
        }

        mVertexBuffer = graphicEngine.CreateVertexBuffer(vertexArray);
    }

    LineShape::LineShape(GraphicEngine& graphicEngine, Array<Vector3<float>> points, Array<LinearColor> colors) :
        mPointCount(points.GetSize())
    {
        assert(colors.GetSize() == points.GetSize());

        VertexLayout::Builder builder;
        builder.AddAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3);
        builder.AddAttribute(VertexAttributeSemantic::Color, DataType::Float32, 4);

        SharedPtr<VertexArray> vertexArray = MakeShared<VertexArray>(builder.Get(), mPointCount);
        for (std::size_t i = 0; i < vertexArray->GetVertexCount(); i++)
        {
            VertexRef vertex = vertexArray->At(i);
            vertex.SetVector(VertexAttributeSemantic::Position, points[i]);
            vertex.SetColor(VertexAttributeSemantic::Color, colors[i]);
        }

        mVertexBuffer = graphicEngine.CreateVertexBuffer(vertexArray);
    }

    void LineShape::AddToQueue(RenderQueue& queue, const Transformation& cameraTransformation, const Transformation& worldTransformation)
    {
        LineRecordInfo recordInfo;
        recordInfo.WorldMatrix = worldTransformation.ToMatrix();
        recordInfo.VertexBuffer = mVertexBuffer->GetUnderlyingResource();
        recordInfo.PointCount = mPointCount;
        recordInfo.Color = Color;
        recordInfo.Thickness = Thickness;
        recordInfo.IsStrip = IsStip;
        recordInfo.IsStipple = IsStipple;
        recordInfo.RasterizationMode = RasterizationMode;
        recordInfo.StippleFactor = StippleFactor;
        recordInfo.StipplePattern = StipplePattern;
        recordInfo.HasVertexColor = mVertexBuffer->GetVertexArray()->GetVertexLayout()->FindAttribute(VertexAttributeSemantic::Color) != nullptr;

        queue.PushLine(recordInfo, Material::ShadingMode::Unlit, 0);
    }

    void LineShape::ExtendBoundingVolume(Volume<float>& volume)
    {
        /// Lines can be on a 2D plane, leading to an invalid bouding volume of the scene node if they are the only shape rendered
        if (volume.IsNull())
            volume.Extend(Vector3<float>::Epsilon());

        SharedPtr<VertexArray> vertexArray = mVertexBuffer->GetVertexArray();
        for (std::size_t i = 0; i < vertexArray->GetVertexCount(); i++)
        {
            VertexRef vertex = vertexArray->At(i);
            Vector3<float> position = vertex.GetVector<Vector3<float>>(VertexAttributeSemantic::Position);

            volume.Extend(position);
        }
    }
}
