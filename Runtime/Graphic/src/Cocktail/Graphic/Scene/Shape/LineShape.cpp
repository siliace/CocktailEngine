#include <Cocktail/Graphic/Scene/Shape/LineShape.hpp>

namespace Ck
{
    LineShape::LineShape(GraphicEngine& graphicEngine, Array<Vector3<float>> points, LinearColor color, float thickness, bool isStrip) :
        LineShape(graphicEngine, std::move(points), Array<LinearColor>(), color, thickness, isStrip)
    {
        /// Nothing
    }

    LineShape::LineShape(GraphicEngine& graphicEngine, Array<Vector3<float>> points, Array<LinearColor> colors, LinearColor color, float thickness, bool isStrip) :
        mPointCount(points.GetSize()),
        mColor(color),
        mThickness(thickness),
        mIsStip(isStrip)
    {
        VertexLayout::Builder builder;
        builder.AddAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3);
        if (!colors.IsEmpty())
        {
            assert(colors.GetSize() == points.GetSize());
            builder.AddAttribute(VertexAttributeSemantic::Color, DataType::Float32, 4);
        }

        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>(builder.Get(), mPointCount);
        for (std::size_t i = 0; i < vertexArray->GetVertexCount(); i++)
        {
            VertexRef vertex = vertexArray->At(i);
            vertex.SetVector(VertexAttributeSemantic::Position, points[i]);
            if (!colors.IsEmpty())
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
        recordInfo.Color = mColor;
        recordInfo.Thickness = mThickness;
        recordInfo.IsStrip = mIsStip;
        recordInfo.HasVertexColor = mVertexBuffer->GetVertexArray()->GetVertexLayout()->FindAttribute(VertexAttributeSemantic::Color) != nullptr;

        queue.PushLine(recordInfo, 0);
    }

    void LineShape::ExtendBoundingVolume(Volume<float>& volume)
    {
        /// Lines can be on a 2D plane, leading to an invalid bouding volume of the scene node if they are the only shape rendered
        if (volume.IsNull())
            volume.Extend(Vector3<float>::Epsilon());

        std::shared_ptr<VertexArray> vertexArray = mVertexBuffer->GetVertexArray();
        for (std::size_t i = 0; i < vertexArray->GetVertexCount(); i++)
        {
            VertexRef vertex = vertexArray->At(i);
            Vector3<float> position = vertex.GetVector<Vector3<float>>(VertexAttributeSemantic::Position);

            volume.Extend(position);
        }
    }
}
