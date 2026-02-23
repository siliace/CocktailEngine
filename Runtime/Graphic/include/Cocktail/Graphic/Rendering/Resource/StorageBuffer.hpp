#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCES_STORAGEBUFFER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCES_STORAGEBUFFER_HPP

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>

namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_GRAPHIC_API StorageBuffer : public BufferResource
    {
    public:

        /**
         * \brief
         *
         * \param graphicEngine
         * \param size
         * \param name
         */
        StorageBuffer(const std::shared_ptr<GraphicEngine>& graphicEngine, std::size_t size, const AnsiChar* name);

        /**
         * \brief
         *
         * \param offset
         * \param length
         * \param data
         */
        void Upload(std::size_t offset, std::size_t length, const void* data);
    };

    template <typename T>
    class TypedStorageBuffer : public StorageBuffer
    {
    public:

        TypedStorageBuffer(const std::shared_ptr<GraphicEngine>& graphicEngine, unsigned int elementCount, const AnsiChar* name) :
            StorageBuffer(graphicEngine, elementCount * sizeof(T), name),
            mElements(elementCount)
        {
            /// Nothing
        }

        T& At(unsigned int index)
        {
            return mElements.At(index);
        }

        const T& At(unsigned int index) const
        {
            return mElements.At(index);
        }

        void Upload()
        {
            StorageBuffer::Upload(0, mElements.GetSize() * sizeof(T), mElements.GetData());
        }

        void Upload(std::size_t firstElement, std::size_t elementCount)
        {
            StorageBuffer::Upload(firstElement * sizeof(T), elementCount * sizeof(T), mElements.GetData());
        }

        unsigned int GetElementCount() const
        {
            return mElements.GetSize();
        }

    private:

        Array<T> mElements;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCES_STORAGEBUFFER_HPP
