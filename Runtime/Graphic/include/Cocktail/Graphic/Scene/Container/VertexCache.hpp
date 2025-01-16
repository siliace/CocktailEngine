#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_VERTEXCACHE_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_VERTEXCACHE_HPP

#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>

namespace Ck
{
    /**
     * \brief Utility class to de-duplicate vertices and generate indexes
     */
    template <typename T, typename Hasher = std::hash<T>, typename Equal = std::equal_to<T>>
    class VertexCache
    {
    public:

        /**
         * \brief Constructor
         * \param vertexLayout
         */
        explicit VertexCache(Ref<VertexLayout> vertexLayout) :
            mVertexLayout(std::move(vertexLayout)),
            mCurrentIndex(0)
        {
            /// Nothing
        }

        virtual ~VertexCache() = default;

        /**
         * \brief Register a new ObjVertex in the cache
         * \param vertex
         */
        void AddVertex(const T& vertex)
        {
            auto [it, inserted] = mVertices.insert({ vertex, mCurrentIndex });
            if (inserted)
                ++mCurrentIndex;

            mIndices.push_back(it->second);
        }

        /**
         * \brief
         * \param other
         */
        void Merge(const VertexCache& other)
        {
            assert(mVertexLayout->IsCompatibleWith(*other.GetVertexLayout()));
            for (const auto& [vertex, index] : other.mVertices)
                AddVertex(vertex);
        }

        /**
         * \brief
         * \return
         */
        Ref<IndexArray> CreateIndexArray() const
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

        /**
         * \brief
         * \return
         */
        Ref<VertexArray> CreateVertexArray() const
        {
            Ref<VertexArray> vertices = VertexArray::New(mVertexLayout, mVertices.size());
            for (const auto& [vertex, index] : mVertices)
                HydrateVertexRef(vertices->At(index), vertex);

            return vertices;
        }

        Ref<VertexLayout> GetVertexLayout() const
        {
            return mVertexLayout;
        }

        /**
         * \brief
         * \return
         */
        std::size_t GetVertexCount() const
        {
            return mVertices.size();
        }

        /**
         * \brief
         * \return
         */
        std::size_t GetIndexCount() const
        {
            return mIndices.size();
        }

    protected:

        /**
         * \brief
         * \param vertexRef
         * \param vertex
         */
        virtual void HydrateVertexRef(VertexRef vertexRef, const T& vertex) const = 0;

    private:

        Ref<VertexLayout> mVertexLayout;
        unsigned int mCurrentIndex;
        std::unordered_map<T, unsigned int, Hasher, Equal> mVertices;
        std::vector<unsigned int> mIndices;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_VERTEXCACHE_HPP