#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJVERTEXCACHE_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJVERTEXCACHE_HPP

#include <Cocktail/Graphic/Scene/Container/VertexCache.hpp>
#include <Cocktail/Graphic/Scene/Container/Obj/ObjVertex.hpp>

namespace Ck
{
    class ObjVertexCache : public VertexCache<ObjVertex, ObjVertex::Hasher, ObjVertex::Comparator>
    {
    public:

        /**
         * \brief Constructor
         * \param vertexLayout
         */
        explicit ObjVertexCache(Ref<VertexLayout> vertexLayout) :
            VertexCache<ObjVertex, ObjVertex::Hasher, ObjVertex::Comparator>(std::move(vertexLayout))
        {
            /// Nothing
        }

    protected:

        void HydrateVertexRef(VertexRef vertexRef, const ObjVertex& vertex) const override
        {
            vertexRef.Set(VertexAttributeSemantic::Position, vertex.Position);
            vertexRef.Set(VertexAttributeSemantic::TexCoord, vertex.TexCoord);
            vertexRef.Set(VertexAttributeSemantic::Normal, vertex.Normal);
        }
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJVERTEXCACHE_HPP