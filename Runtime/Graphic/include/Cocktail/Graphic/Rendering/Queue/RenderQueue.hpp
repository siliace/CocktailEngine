#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP

#include <Cocktail/Core/Log/LogCategory.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>
#include <Cocktail/Graphic/Rendering/Queue/CustomRecordInfo.hpp>
#include <Cocktail/Graphic/Rendering/Queue/LineRecordInfo.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp>
#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecordInfo.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_LOG_CATEGORY(RenderQueueLogCategory, Ck::LogLevel::Info);

    /**
     * \brief Collects and sorts render records before submission to the renderer
     *
     * RenderQueue is responsible for gathering renderable records (meshes, lines,
     * custom primitives, etc.), sorting them using a user-provided sorting key,
     * and flushing them to a command list during rendering.
     *
     * A RenderQueue instance is typically configured for a specific blending mode (e.g. opaque or transparent pass).
     */
    class COCKTAIL_GRAPHIC_API RenderQueue
    {
    public:

        /**
         * \brief Defines how blending is handled for records in this queue
         */
        enum class BlendingMode
        {
            Opaque, /*!< Render only opaque objects */
            Transparent /*!< Render transparent objects (typically sorted back-to-front) */,
            Ignore, /*!< Do not enforce blending filtering */
        };

        /**
         * \brief Constructs a render queue
         *
         * \param materialProgramManager Manager used to retrieve GPU programs compatible with the selected shading mode
         * \param blendingMode The blending mode filter applied to this queue
         */
        RenderQueue(std::shared_ptr<MaterialProgramManager> materialProgramManager, BlendingMode blendingMode = BlendingMode::Ignore);

        /**
         * \brief Pushes a custom render record into the queue
         *
         * \param recordInfo The custom record information describing the draw call
         * \param sortingKey Key used to sort the record before rendering
         */
        void PushCustom(const CustomRecordInfo& recordInfo, Uint64 sortingKey);

        /**
         * \brief Pushes a static mesh render record into the queue
         *
         * \param recordInfo The static mesh record information
         * \param shadingMode The shading mode to use to render the record
         * \param sortingKey Key used to sort the record before rendering
         */
        void PushStaticMesh(const StaticMeshRecordInfo& recordInfo, Material::ShadingMode shadingMode, Uint64 sortingKey);

        /**
         * \brief Pushes a line render record into the queue
         *
         * \param recordInfo The line record information
         * \param shadingMode The shading mode to use to render the record
         * \param sortingKey Key used to sort the record before rendering
         */
        void PushLine(const LineRecordInfo& recordInfo, Material::ShadingMode shadingMode, Uint64 sortingKey);

        /**
         * \brief Sorts and flushes all queued records to the command list
         *
         * Records are sorted according to their sorting keys before being
         * submitted. The appropriate GPU programs and states are selected
         * based on the queue configuration.
         *
         * \param commandList The command list used to record draw commands
         * \param drawContext Additional draw context shared across records
         */
        void Flush(Renderer::CommandList& commandList, RecordDrawContext& drawContext);

        /**
         * \brief Clears all queued records
         *
         * Does not affect the queue configuration.
         */
        void Clear();

        /**
         * \brief Gets the blending mode of the queue
         *
         * \return The blending mode
         */
        BlendingMode GetBlendingMode() const;

    private:

        /**
         * \brief Internal representation of a queued render record
         */
        struct RecordInfo
        {
            std::shared_ptr<RenderRecord> Record; /*!< The render record to execute */
            Uint64 SortingKey = 0; /*!< Sorting key used to order records before flushing */
        };

        /**
         * \brief Inserts a render record into the queue.
         *
         * \param record The render record to store.
         * \param sortingKey Key used to sort the record.
         */
        void Emplace(std::shared_ptr<RenderRecord> record, Uint64 sortingKey);

        MaterialProgramVariant* FindMaterialProgramVariant(RenderableType renderable, Material::ShadingMode shadingMode, Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const;

        BlendingMode mBlendingMode; /*!< Blending mode filter applied to this queue */
        std::shared_ptr<MaterialProgramSet> mMaterialProgramSet; /*!< Set of material programs compatible with the queue configuration */
        Array<RecordInfo> mRecords; /*!< Collected render records waiting to be flushed */
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
