#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJSCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJSCENECONTAINER_HPP

#include <tiny_obj_loader.h>

#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>
#include <Cocktail/Graphic/Scene/Container/Obj/ObjVertexCache.hpp>

namespace Ck
{
	class ObjSceneContainer : public SceneContainer
	{
	public:

		ObjSceneContainer(const SceneImportParameters& importParameters, const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials);

	private:

		SharedPtr<MipMaps> LoadMipMaps(const Path& path);
		MaterialInfo ProcessMaterial(const tinyobj::material_t& objMaterial);

		void ProcessIndices(ObjVertexCache& vertexCache, const tinyobj::attrib_t& attributes, const tinyobj::index_t* indices, std::size_t indicesCount) const;
		bool ProcessSubMesh(ObjVertexCache& vertexCache, Array<SubMeshInfo>& subMeshes, const tinyobj::attrib_t& attributes, const tinyobj::shape_t& shape) const;
		MeshInfo ProcessMesh(const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes);

		SharedPtr<VertexLayout> mVertexLayout;
		SceneImportParameters mImportParameters;
		HashMap<Path, SharedPtr<MipMaps>> mMipMaps;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJSCENECONTAINER_HPP