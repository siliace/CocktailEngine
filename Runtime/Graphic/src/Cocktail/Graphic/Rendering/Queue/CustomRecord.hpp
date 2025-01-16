#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Rendering/Queue/CustomRecordInfo.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp>

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
		static Ref<CustomRecord> New(const CustomRecordInfo& recordInfo);

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

		CustomRecordInfo mRecordInfo;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP