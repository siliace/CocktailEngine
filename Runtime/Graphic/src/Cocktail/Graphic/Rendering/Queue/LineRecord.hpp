#ifndef COCKTAILENGINE_LINERECORD_HPP
#define COCKTAILENGINE_LINERECORD_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Graphic/Rendering/Queue/LineRecordInfo.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp>

#include "Cocktail/Core/Math/Matrix/Matrix4.hpp"

namespace Ck
{
    class LineRecord : public RenderRecord
    {
    public:

        static std::shared_ptr<LineRecord> New(const LineRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

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
