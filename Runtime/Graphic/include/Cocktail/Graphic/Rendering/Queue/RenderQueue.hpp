#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP

#include <queue>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>
#include <Cocktail/Graphic/Rendering/Queue/CustomRecordInfo.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp>
#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecordInfo.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API RenderQueue : public Extends<RenderQueue, Object>
	{
	public:

		/**
		 * \brief 
		 * \param materialProgramManager 
		 * \param shadingMode 
		 */
		RenderQueue(Ref<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode);

		/**
		 * \brief 
		 * \param recordInfo 
		 */
		void PushCustom(const CustomRecordInfo& recordInfo, Uint64 sortingKey);

		/**
		 * \brief 
		 * \param recordInfo 
		 */
		void PushStaticMesh(const StaticMeshRecordInfo& recordInfo, Uint64 sortingKey);

		/**
		 * \brief 
		 * \param commandList 
		 * \param drawContext 
		 */
		void Flush(Renderer::CommandList& commandList, RecordDrawContext& drawContext);

		/**
		 * \brief 
		 */
		void Clear();

		/**
		 * \brief 
		 * \return 
		 */
		Material::ShadingMode GetShadingMode() const;

	private:

		struct RecordInfo
		{
			Ref<RenderRecord> Record;
			Uint64 SortingKey = 0;
		};

		void Emplace(Ref<RenderRecord> record, Uint64 sortingKey);

		Material::ShadingMode mShadingMode;
		Ref<MaterialProgramSet> mMaterialProgramSet;
		std::vector<RecordInfo> mRecords;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
