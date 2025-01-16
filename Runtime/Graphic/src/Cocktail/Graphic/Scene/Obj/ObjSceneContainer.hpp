#ifndef COCKTAIL_GRAPHIC_SCENE_OBJ_OBJSCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_OBJ_OBJSCENECONTAINER_HPP

#include <tiny_obj_loader.h>

#include <Cocktail/Graphic/Scene/SceneContainer.hpp>
#include <Cocktail/Graphic/Scene/Obj/ObjVertexCache.hpp>

namespace Ck
{
	class ObjSceneContainer : public Extends<ObjSceneContainer, SceneContainer>
	{
	public:

		ObjSceneContainer(const SceneImportParameters& importParameters, const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials);

	private:

		Ref<MipMaps> LoadMipMaps(const std::filesystem::path& path);
		MaterialInfo ProcessMaterial(const tinyobj::material_t& objMaterial);

		Ref<ObjVertexCache> CreateVertexCache(const tinyobj::attrib_t& attributes, const std::vector<tinyobj::index_t>& indices) const;
		MeshInfo ProcessShape(const tinyobj::attrib_t& attributes, const tinyobj::shape_t& objShape);

		Ref<VertexLayout> mVertexLayout;
		SceneImportParameters mImportParameters;
		CachePool<std::filesystem::path, Ref<MipMaps>> mMipMaps;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_OBJ_OBJSCENECONTAINER_HPP