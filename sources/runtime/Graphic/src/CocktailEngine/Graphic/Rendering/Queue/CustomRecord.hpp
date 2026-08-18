#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORD_HPP

#include <CocktailEngine/Core/Utility/ObjectPool.hpp>

#include <CocktailEngine/Graphic/Rendering/Queue/CustomRecordInfo.hpp>
#include <CocktailEngine/Graphic/Rendering/Queue/RenderRecord.hpp>

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
		static SharedPtr<CustomRecord> New(const CustomRecordInfo& recordInfo);

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