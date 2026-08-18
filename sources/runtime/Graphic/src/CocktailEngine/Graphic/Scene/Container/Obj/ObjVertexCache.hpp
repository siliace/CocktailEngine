#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJVERTEXCACHE_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJVERTEXCACHE_HPP

#include <CocktailEngine/Graphic/Scene/Container/VertexCache.hpp>
#include <CocktailEngine/Graphic/Scene/Container/Obj/ObjVertex.hpp>

namespace Ck
{
    class ObjVertexCache : public VertexCache<ObjVertex, ObjVertex::Hasher, ObjVertex::Comparator>
    {
    public:

        /**
         * \brief Constructor
         * \param vertexLayout
         */
        explicit ObjVertexCache(SharedPtr<VertexLayout> vertexLayout) :
            VertexCache<ObjVertex, ObjVertex::Hasher, ObjVertex::Comparator>(Move(vertexLayout))
        {
            /// Nothing
        }

    protected:

        void HydrateVertexRef(VertexRef vertexRef, const ObjVertex& vertex) const override
        {
            vertexRef.SetVector(VertexAttributeSemantic::Position, vertex.Position);
            vertexRef.SetVector(VertexAttributeSemantic::TexCoord, vertex.TexCoord);
            vertexRef.SetVector(VertexAttributeSemantic::Normal, vertex.Normal);
        }
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJVERTEXCACHE_HPP