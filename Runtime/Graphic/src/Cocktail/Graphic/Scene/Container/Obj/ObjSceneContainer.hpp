#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJSCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJSCENECONTAINER_HPP

#include <tiny_obj_loader.h>

#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>
#include <Cocktail/Graphic/Scene/Container/Obj/ObjVertexCache.hpp>

namespace Ck
{
	class ObjSceneContainer : public Extends<ObjSceneContainer, SceneContainer>
	{
	public:

		/**
		 * \brief Create a material to be served a default one for shape that have none (like point or line shapes)
		 * \param baseColor The base color of the material to create
		 * \return The created material
		 */
		static MaterialInfo CreateDefaultMaterial(LinearColor baseColor);

		ObjSceneContainer(const SceneImportParameters& importParameters, const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials);

	private:


		Ref<MipMaps> LoadMipMaps(const std::filesystem::path& path);
		MaterialInfo ProcessMaterial(const tinyobj::material_t& objMaterial);

		void ProcessIndices(ObjVertexCache& vertexCache, const tinyobj::attrib_t& attributes, const tinyobj::index_t* indices, std::size_t indicesCount) const;
		bool ProcessSubMesh(ObjVertexCache& vertexCache, std::vector<SubMeshInfo>& subMeshes, const tinyobj::attrib_t& attributes, const tinyobj::shape_t& shape);
		MeshInfo ProcessMesh(const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes);

		Ref<VertexLayout> mVertexLayout;
		SceneImportParameters mImportParameters;
		std::unordered_map<std::filesystem::path, Ref<MipMaps>> mMipMaps;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJSCENECONTAINER_HPP