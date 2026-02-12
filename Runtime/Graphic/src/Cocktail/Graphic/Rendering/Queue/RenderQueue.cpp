#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Rendering/Queue/CustomRecord.hpp>
#include <Cocktail/Graphic/Rendering/Queue/LineRecord.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>
#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.hpp>

namespace Ck
{
	COCKTAIL_DEFINE_LOG_CATEGORY(RenderQueueLogCategory);

	RenderQueue::RenderQueue(std::shared_ptr<MaterialProgramManager> materialProgramManager, BlendingMode blendingMode) :
		mBlendingMode(blendingMode)
	{
		mMaterialProgramSet = std::make_shared<MaterialProgramSet>(materialProgramManager);
	}

	void RenderQueue::PushCustom(const CustomRecordInfo& recordInfo, Uint64 sortingKey)
	{
		Emplace(CustomRecord::New(recordInfo), sortingKey);
	}

	void RenderQueue::PushStaticMesh(const StaticMeshRecordInfo& recordInfo, Material::ShadingMode shadingMode, Uint64 sortingKey)
	{
		// Todo: move this outside of RenderQueue class
		Flags<VertexAttributeSemantic> vertexAttributes;
		for (unsigned int i = 0; i < recordInfo.VertexBufferCount; i++)
		{
			for (VertexAttributeSemantic attribute : Enum<VertexAttributeSemantic>::Values)
			{
				if (!recordInfo.VertexBuffers[i].VertexLayout->FindAttribute(attribute))
					continue;

				if (vertexAttributes & attribute)
				{
					CK_LOG(RenderQueueLogCategory, LogLevel::Error, CK_TEXT("Duplicated VertexAttribute %hs in VertexBuffer %d of StaticMeshRecordInfo"), attribute, i);
					return;
				}
				vertexAttributes |= attribute;
			}
		}

		Flags<Material::TextureType> materialTextures;
		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			if (recordInfo.MaterialTextures[textureType] == nullptr)
				continue;

			materialTextures |= textureType;
		}

	    if (MaterialProgramVariant* variant = FindMaterialProgramVariant(RenderableType::Mesh, shadingMode, vertexAttributes, materialTextures))
		    Emplace(StaticMeshRecord::New(recordInfo, variant), sortingKey);
	}

    void RenderQueue::PushLine(const LineRecordInfo& recordInfo, Material::ShadingMode shadingMode, Uint64 sortingKey)
    {
	    Flags<VertexAttributeSemantic> vertexAttributes = VertexAttributeSemantic::Position;
	    if (recordInfo.HasVertexColor)
	        vertexAttributes |= VertexAttributeSemantic::Color;

	    if (MaterialProgramVariant* variant = FindMaterialProgramVariant(RenderableType::Line, shadingMode, vertexAttributes, {}))
	        Emplace(LineRecord::New(recordInfo, variant), sortingKey);
    }

    void RenderQueue::Flush(Renderer::CommandList& commandList, RecordDrawContext& drawContext)
	{
		std::sort(begin(mRecords), end(mRecords), [](const RecordInfo& lhs, const RecordInfo& rhs) {
			return lhs.SortingKey < rhs.SortingKey;
		});

		for (const RecordInfo& recordInfo : mRecords)
			recordInfo.Record->Draw(commandList, drawContext);

		mRecords.Clear();
	}

	void RenderQueue::Clear()
	{
		mRecords.Clear();
	}

	RenderQueue::BlendingMode RenderQueue::GetBlendingMode() const
	{
		return mBlendingMode;
	}

	void RenderQueue::Emplace(std::shared_ptr<RenderRecord> record, Uint64 sortingKey)
	{
		mRecords.Emplace(RecordInfo{ std::move(record), sortingKey });
	}

    MaterialProgramVariant* RenderQueue::FindMaterialProgramVariant(RenderableType renderable, Material::ShadingMode shadingMode, Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const
	{
	    MaterialProgram* materialProgram = mMaterialProgramSet->GetMaterialProgram(renderable, shadingMode);
	    if (!materialProgram)
	    {
	        CK_LOG(RenderQueueLogCategory, LogLevel::Error, CK_TEXT("No MaterialProgram found for renderable %s in shading mode %s"), renderable, shadingMode);
	        return nullptr;
	    }

	    MaterialProgramVariant* materialProgramVariant = materialProgram->GetVariant(vertexAttributes, materialTextures);
	    if (!materialProgramVariant)
	    {
	        CK_LOG(RenderQueueLogCategory, LogLevel::Error, CK_TEXT("No variant found in material program %s with usage mask %d and %d"), materialProgram->GetName(), vertexAttributes, materialTextures);
	        return nullptr;
	    }

	    return materialProgramVariant;
    }
}
