#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Graphic/Rendering/Queue/CustomRecord.hpp>

namespace Ck
{
	std::shared_ptr<CustomRecord> CustomRecord::New(const CustomRecordInfo& recordInfo)
	{
		static ObjectPool<CustomRecord> RecordPool;
		return RecordPool.Allocate(recordInfo);
	}

    CustomRecord::CustomRecord(const CustomRecordInfo& recordInfo) :
		mRecordInfo(recordInfo)
	{
		/// Nothing
	}

	void CustomRecord::Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const 
	{
		assert(mRecordInfo.Draw);
		mRecordInfo.Draw(commandList, mRecordInfo.UserData, drawContext);
	}
}