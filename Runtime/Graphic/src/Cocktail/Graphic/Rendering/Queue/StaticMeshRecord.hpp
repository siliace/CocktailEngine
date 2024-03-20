#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORD_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class StaticMeshRecord : public Inherit<StaticMeshRecord, Object, RenderRecord>
	{
	public:

		/**
		 * \brief 
		 * \param recordInfo 
		 * \return 
		 */
		static Ref<StaticMeshRecord> New(const StaticMeshRecordInfo& recordInfo);

		/**
		 * \brief 
		 * \param recordInfo 
		 */
		explicit StaticMeshRecord(const StaticMeshRecordInfo& recordInfo);

		/**
		 * \brief 
		 * \param commandList 
		 * \param drawContext 
		 */
		void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const override;

	private:

		StaticMeshRecordInfo mRecordInfo;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORD_HPP
