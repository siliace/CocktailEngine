#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Rendering/Queue/CustomRecord.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>
#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.hpp>

namespace Ck
{
	RenderQueue::RenderQueue(std::shared_ptr<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode, BlendingMode blendingMode) :
		mShadingMode(shadingMode),
		mBlendingMode(blendingMode)
	{
		mMaterialProgramSet = std::make_shared<MaterialProgramSet>(materialProgramManager, mShadingMode);
	}

	void RenderQueue::PushCustom(const CustomRecordInfo& recordInfo, Uint64 sortingKey)
	{
		Emplace(CustomRecord::New(recordInfo), sortingKey);
	}

	void RenderQueue::PushStaticMesh(const StaticMeshRecordInfo& recordInfo, Uint64 sortingKey)
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
					Log::Error("Duplicated VertexAttribute {} in VertexBuffer {} of StaticMeshRecordInfo", Enum<VertexAttributeSemantic>::ToString(attribute), i);
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

		std::shared_ptr<MaterialProgramVariant> materialProgramVariant = mMaterialProgramSet->GetMaterialProgram(RenderableType::Mesh)->GetVariant(vertexAttributes, materialTextures);
		if (!materialProgramVariant)
		{
			Log::Error("No MaterialProgram found for Material");
			return;
		}

		Emplace(StaticMeshRecord::New(recordInfo, materialProgramVariant.get()), sortingKey);
	}

	void RenderQueue::Flush(Renderer::CommandList& commandList, RecordDrawContext& drawContext)
	{
		std::sort(mRecords.begin(), mRecords.end(), [](const RecordInfo& lhs, const RecordInfo& rhs) {
			return lhs.SortingKey < rhs.SortingKey;
		});

		for (const RecordInfo& recordInfo : mRecords)
			recordInfo.Record->Draw(commandList, drawContext);

		mRecords.clear();
	}

	void RenderQueue::Clear()
	{
		mRecords.clear();
	}

	RenderQueue::BlendingMode RenderQueue::GetBlendingMode() const
	{
		return mBlendingMode;
	}

	Material::ShadingMode RenderQueue::GetShadingMode() const
	{
		return mShadingMode;
	}

	void RenderQueue::Emplace(std::shared_ptr<RenderRecord> record, Uint64 sortingKey)
	{
		mRecords.emplace_back(RecordInfo{ std::move(record), sortingKey });
	}
}
