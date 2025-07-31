#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORD_HPP

#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class StaticMeshRecord : public RenderRecord
	{
	public:

		/**
		 * \brief 
		 * \param recordInfo 
		 * \return 
		 */
		static std::shared_ptr<StaticMeshRecord> New(const StaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

		/**
		 * \brief 
		 * \param recordInfo 
		 */
		StaticMeshRecord(const StaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

		/**
		 * \brief 
		 * \param commandList 
		 * \param drawContext 
		 */
		void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const override;

	private:

		friend ObjectPool<StaticMeshRecord>;
		static ObjectPool<StaticMeshRecord> sRecordPool;

		struct VertexInfo
		{
			Matrix4<float> Model;
			Vector4<float> Normal[3]; // Alignment mess
		};
		
		struct MaterialInfo
		{
			LinearColor BaseColor;
			float SpecularColor[3];
			int AlphaMode;
			float EmissiveColor[3];
			float AlphaCutoff;
		};

		/**
		 * \brief Default constructor
		 */
		StaticMeshRecord() = default;

		StaticMeshRecordInfo mRecordInfo;
		MaterialProgramVariant* mMaterialProgramVariant;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORD_HPP
