#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_LINERECORDINFO_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_LINERECORDINFO_HPP

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Math/Matrix/Matrix4.hpp>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>

namespace Ck
{
    struct LineRecordInfo
    {
        Matrix4<float> WorldMatrix;

        Renderer::Buffer* VertexBuffer = nullptr;

        unsigned int PointCount = 0;

        LinearColor Color = LinearColor::White;

        float Thickness = 1.f;

        bool IsStrip = false;

        bool HasVertexColor = false;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_LINERECORDINFO_HPP
