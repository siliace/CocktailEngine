#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Graphic/Rendering/Queue/CustomRecordInfo.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp>

namespace Ck
{
    class CustomRecord : public RenderRecord
	{
	public:

		/**
		 * \brief
		 * \param recordInfo
		 * \return
		 */
		static std::shared_ptr<CustomRecord> New(const CustomRecordInfo& recordInfo);

		/**
		 * \brief
		 * \param recordInfo
		 */
		explicit CustomRecord(const CustomRecordInfo& recordInfo);

		/**
		 * \brief 
		 * \param commandList 
		 * \param drawContext 
		 */
		void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const override;

	private:

		friend ObjectPool<CustomRecord>;
		static ObjectPool<CustomRecord> sRecordPool;

		/**
		 * \brief Default constructor
		 */
		CustomRecord() = default;

		CustomRecordInfo mRecordInfo;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP