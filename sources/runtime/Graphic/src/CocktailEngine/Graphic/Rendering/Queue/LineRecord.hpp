#ifndef COCKTAILENGINE_LINERECORD_HPP
#define COCKTAILENGINE_LINERECORD_HPP

#include <CocktailEngine/Core/Math/Matrix/Matrix4.hpp>
#include <CocktailEngine/Core/Utility/ObjectPool.hpp>

#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramVariant.hpp>
#include <CocktailEngine/Graphic/Rendering/Queue/LineRecordInfo.hpp>
#include <CocktailEngine/Graphic/Rendering/Queue/RenderRecord.hpp>

namespace Ck
{
    class LineRecord : public RenderRecord
    {
    public:

        static SharedPtr<LineRecord> New(const LineRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

        LineRecord(const LineRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

        void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const override;

    private:

        struct VertexInfo
        {
            Matrix4<float> Model;
        };

        struct FragmentInfo
        {
            LinearColor Color;
        };

        static ObjectPool<LineRecord> sRecordPool;

        LineRecordInfo mRecordInfo;
        MaterialProgramVariant* mMaterialProgramVariant;
    };
}

#endif // COCKTAILENGINE_LINERECORD_HPP
