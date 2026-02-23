#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_INSTANCEDSTATICMESHRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_INSTANCEDSTATICMESHRECORD_HPP

#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class InstancedStaticMeshRecord : public RenderRecord
	{
	public:

		/**
		 * \brief 
		 * \param recordInfo
		 * \param materialProgramVariant
		 * \return 
		 */
		static std::shared_ptr<InstancedStaticMeshRecord> New(const InstancedStaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

		/**
		 * \brief 
		 * \param recordInfo
		 * \param materialProgramVariant
		 */
		InstancedStaticMeshRecord(const InstancedStaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

		/**
		 * \brief 
		 * \param commandList 
		 * \param drawContext 
		 */
		void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const override;

	private:

		friend ObjectPool<InstancedStaticMeshRecord>;
		static ObjectPool<InstancedStaticMeshRecord> sRecordPool;

		struct VertexInfo
		{
			Matrix4<float> Model;
			Vector4<float> Normal[3]; // Alignment mess
		};
		
		struct MaterialInfo
		{
			LinearColor BaseColor;
			float EmissiveColor[3];
		    float Roughness;
		    float Metallic;
			int AlphaMode;
			float AlphaCutoff;
		};

		/**
		 * \brief Default constructor
		 */
		InstancedStaticMeshRecord() = default;

		InstancedStaticMeshRecordInfo mRecordInfo;
		MaterialProgramVariant* mMaterialProgramVariant;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_INSTANCEDSTATICMESHRECORD_HPP
