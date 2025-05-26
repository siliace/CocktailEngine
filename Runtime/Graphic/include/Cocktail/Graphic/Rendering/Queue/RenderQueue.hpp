#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP

#include <queue>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>
#include <Cocktail/Graphic/Rendering/Queue/CustomRecordInfo.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp>
#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecordInfo.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API RenderQueue
	{
	public:

		enum class BlendingMode
		{
			Opaque,
			Transparent,
			Ignore,
		};

		/**
		 * \brief 
		 * \param materialProgramManager 
		 * \param shadingMode 
		 */
		RenderQueue(std::shared_ptr<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode, BlendingMode blendingMode = BlendingMode::Ignore);

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
		BlendingMode GetBlendingMode() const;

		/**
		 * \brief 
		 * \return 
		 */
		Material::ShadingMode GetShadingMode() const;

	private:

		struct RecordInfo
		{
			std::shared_ptr<RenderRecord> Record;
			Uint64 SortingKey = 0;
		};

		void Emplace(std::shared_ptr<RenderRecord> record, Uint64 sortingKey);

		Material::ShadingMode mShadingMode;
		BlendingMode mBlendingMode;
		std::shared_ptr<MaterialProgramSet> mMaterialProgramSet;
		std::vector<RecordInfo> mRecords;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERQUEUE_HPP
