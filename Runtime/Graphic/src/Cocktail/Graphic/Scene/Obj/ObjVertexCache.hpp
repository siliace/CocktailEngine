#ifndef COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEXCACHE_HPP
#define COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEXCACHE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>
#include <Cocktail/Graphic/Scene/Obj/ObjVertex.hpp>

namespace Ck
{
    /**
     * \brief Utility class to de-duplicate vertices and generate indexes
     */
    class ObjVertexCache : public Extends<ObjVertexCache, Object>
    {
    public:

        /**
         * \brief Constructor
         * \param vertexLayout
         * \param vertexCount
         */
        ObjVertexCache(Ref<VertexLayout> vertexLayout, std::size_t vertexCount);

        /**
         * \brief Register a new ObjVertex in the cache
         * \param objVertex
         */
        void AddVertex(const ObjVertex& objVertex);

        /**
         * \brief
         * \return
         */
        Ref<IndexArray> CreateIndexArray() const;

        /**
         * \brief
         * \return
         */
        Ref<VertexArray> CreateVertexArray() const;

    private:

		struct ObjVertexHasher
		{
			std::size_t operator()(const ObjVertex& objVertex) const noexcept
			{
				std::size_t positionHash = 0;
				for (auto i = 0; i < objVertex.Position.GetSize(); i++)
					HashCombine(positionHash, objVertex.Position.At(i));

				std::size_t texCoordHash = 0;
				for (auto i = 0; i < objVertex.TexCoord.GetSize(); i++)
					HashCombine(texCoordHash, objVertex.TexCoord.At(i));

				std::size_t normalHash = 0;
				for (auto i = 0; i < objVertex.Normal.GetSize(); i++)
					HashCombine(normalHash, objVertex.Normal.At(i));
                    
				return HashMerge(positionHash, HashMerge(texCoordHash, normalHash));
			}
		};

        struct ObjVertexComparator
		{
			bool operator()(const ObjVertex& lhs, const ObjVertex& rhs) const noexcept
			{
				return std::tie(lhs.Position, lhs.TexCoord, lhs.Normal) == std::tie(rhs.Position, rhs.TexCoord, rhs.Normal);
			}
		};

        Ref<VertexLayout> mVertexLayout;
        unsigned int mCurrentIndex;
        std::unordered_map<ObjVertex, unsigned int, ObjVertexHasher, ObjVertexComparator> mVertices;
        std::vector<unsigned int> mIndices;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEXCACHE_HPP