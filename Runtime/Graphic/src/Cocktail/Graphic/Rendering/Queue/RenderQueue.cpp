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

	void RenderQueue::PushCustom(const CustomRecordInfo& recordInfo)
	{
		mRecordQueue.emplace(CustomRecord::New(recordInfo));
	}

	void RenderQueue::PushStaticMesh(const StaticMeshRecordInfo& recordInfo)
	{
		mRecordQueue.emplace(StaticMeshRecord::New(recordInfo));
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
}
