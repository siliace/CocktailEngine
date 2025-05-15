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
		static Ref<StaticMeshRecord> New(const StaticMeshRecordInfo& recordInfo, MaterialProgramVariant* materialProgramVariant);

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

		struct VertexInfo
		{
			Matrix4<float> Model;
			Vector4<float> Normal[3]; // Alignment mess
		};
		
		struct MaterialColors
		{
			LinearColor Base;
			LinearColor Specular;
			LinearColor Emissive;
		};

		StaticMeshRecordInfo mRecordInfo;
		MaterialProgramVariant* mMaterialProgramVariant;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_STATICMESHRECORD_HPP
