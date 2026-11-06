#ifndef COCKTAILENGINE_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Enum.hpp>
#include <CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp>
#include <CocktailEngine/Core/Utility/EnumMap.hpp>

#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>

#include <CocktailEngine/Vulkan/Queue/QueueFamily.hpp>
#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
    using QueueFamilyIndexList = Array<unsigned int, LinearAllocator<Enum<Renderer::CommandQueueType>::ValueCount>>;

    /**
     * \brief
     */
    class QueueFamilyContext
    {
    public:

        /**
         * \brief
         * \param physicalDevice
         */
        explicit QueueFamilyContext(VkPhysicalDevice physicalDevice);

        /**
         * \brief Tell whether the queue context is unified
         * A QueueFamilyContext is unified when all its families are the same
         * \return
         */
        bool IsUnified() const;

        /**
         * \brief
         * \param unique
         * \return
         */
        QueueFamilyIndexList FindFamilyIndexes(bool unique = true) const;

        /**
         * \brief
         * \tparam Callable
         * \param callable
         * \param unique
         * \return
         */
        template <typename Callable>
        QueueFamilyIndexList FindFamilyIndexes(Callable&& callable, bool unique = true) const
        {
            QueueFamilyIndexList queueIndexes;
            for (Renderer::CommandQueueType queueFamilyType : Enum<Renderer::CommandQueueType>::Values)
            {
                const QueueFamily& family = GetFamily(queueFamilyType);
                unsigned int queueFamilyIndex = family.GetIndex();

                if (!callable(family))
                    continue;

                if (!unique || !queueIndexes.Contains(queueFamilyIndex))
                    queueIndexes.Add(queueFamilyIndex);
            }

            return queueIndexes;
        }

        /**
         * \brief
         * \param type
         * \return
         */
        const QueueFamily& GetFamily(Renderer::CommandQueueType type) const;

        /**
         * \brief
         * \return
         */
        const QueueFamily& GetPresentationQueueFamily() const;

    private:

        /**
         * \brief
         * \param properties
         * \return
         */
        static QueueFamily SelectGraphicQueueFamily(const Array<VkQueueFamilyProperties>& properties);

        /**
         * \brief
         * \param properties
         * \return
         */
        static QueueFamily SelectTransferQueueFamily(const Array<VkQueueFamilyProperties>& properties);

        /**
         * \brief
         * \param properties
         * \return
         */
        static QueueFamily SelectComputeQueueFamily(const Array<VkQueueFamilyProperties>& properties);

        VkPhysicalDevice mPhysicalDevice;
        bool mUnified;
        EnumMap<Renderer::CommandQueueType, QueueFamily> mFamilies;
    };
}

#endif // COCKTAILENGINE_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP
