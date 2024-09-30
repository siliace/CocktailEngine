#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>
#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.hpp>

namespace Ck
{
	class CustomRecord : public Inherit<CustomRecord, Object, RenderRecord>
	{
	public:

		/**
		 * \brief
		 * \param recordInfo
		 * \return
		 */
		static Ref<CustomRecord> New(const CustomRecordInfo& recordInfo)
		{
			static ObjectPool<CustomRecord> RecordPool;
			return RecordPool.Allocate(recordInfo);
		}

		/**
		 * \brief
		 * \param recordInfo
		 */
		explicit CustomRecord(const CustomRecordInfo& recordInfo) :
			mRecordInfo(recordInfo)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param commandList 
		 * \param drawContext 
		 */
		void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const override
		{
			assert(mRecordInfo.Draw);
			mRecordInfo.Draw(commandList, mRecordInfo.UserData, drawContext);
		}

	private:

		CustomRecordInfo mRecordInfo;
	};

	RenderQueue::RenderQueue(Ref<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode) :
		mShadingMode(shadingMode)
	{
		for (RenderableType renderableType : Enum<RenderableType>::Values)
			mMaterialProgramSets[renderableType] = MaterialProgramSet::New(materialProgramManager, renderableType, mShadingMode);
	}

	void RenderQueue::PushCustom(const CustomRecordInfo& recordInfo)
	{
		mRecordQueue.emplace(CustomRecord::New(recordInfo));
	}

	void RenderQueue::PushStaticMesh(const StaticMeshRecordInfo& recordInfo)
	{
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

		Ref<MaterialProgramSet>& materialProgramSet = mMaterialProgramSets[RenderableType::Mesh];
		Ref<MaterialProgram> materialProgram = materialProgramSet->FindMaterialProgram(vertexAttributes, materialTextures);
		if (!materialProgram)
		{
			Log::Error("No MaterialProgram found for Material");
			return;
		}

		mRecordQueue.emplace(StaticMeshRecord::New(recordInfo, materialProgram.Get()));
	}

	void RenderQueue::Flush(Renderer::CommandList& commandList, RecordDrawContext& drawContext)
	{
		while (!mRecordQueue.empty())
		{
			Ref<RenderRecord> record = std::move(mRecordQueue.front());
			mRecordQueue.pop();

			record->Draw(commandList, drawContext);
		}
	}

	void RenderQueue::Clear()
	{
		mRecordQueue = {};
	}

	Material::ShadingMode RenderQueue::GetShadingMode() const
	{
		return mShadingMode;
	}
}
