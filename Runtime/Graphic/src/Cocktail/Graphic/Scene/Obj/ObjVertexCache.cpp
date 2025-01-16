#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>
#include <Cocktail/Graphic/Scene/Obj/ObjVertexCache.hpp>

namespace Ck
{
    ObjVertexCache::ObjVertexCache(Ref<VertexLayout> vertexLayout, std::size_t vertexCount) : 
        mVertexLayout(std::move(vertexLayout)),
        mCurrentIndex(0)
    {
        mVertices.reserve(vertexCount);
        mIndices.reserve(vertexCount);
    }

    void ObjVertexCache::AddVertex(const ObjVertex& objVertex)
    {
        auto [it, inserted] = mVertices.insert({objVertex, mCurrentIndex});
        if (inserted)
            ++mCurrentIndex;

        mIndices.push_back(it->second);
    }

    Ref<IndexArray> ObjVertexCache::CreateIndexArray() const
    {
        const bool useShortIndices = mVertices.size() < std::numeric_limits<Uint16>::max();
        const Renderer::IndexType indexType = useShortIndices ? Renderer::IndexType::Short : Renderer::IndexType::Integer;

        Ref<IndexArray> indices = IndexArray::New(indexType, mIndices.size());
        for (std::size_t i = 0; i < mIndices.size(); i++)
        {
            IndexRef indexRef = indices->At(i);
            unsigned int value = mIndices[i];
            if (useShortIndices)
            {
                indexRef.Set<Uint16>(static_cast<Uint16>(value));
            }
            else
            {
                indexRef.Set<Uint32>(value);
            }
        }

        return indices;
    }

    Ref<VertexArray> ObjVertexCache::CreateVertexArray() const
    {
        Ref<VertexArray> vertices = VertexArray::New(mVertexLayout, mVertices.size());
        for (const auto &[vertex, index] : mVertices)
        {
            VertexRef vertexRef = vertices->At(index);
            vertexRef.Set(VertexAttributeSemantic::Position, vertex.Position);
            vertexRef.Set(VertexAttributeSemantic::TexCoord, vertex.TexCoord);
            vertexRef.Set(VertexAttributeSemantic::Normal, vertex.Normal);
        }

        return vertices;
    }
}