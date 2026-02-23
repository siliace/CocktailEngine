#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_INSTANCEDSTATICMESHRECORDINFO_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_INSTANCEDSTATICMESHRECORDINFO_HPP

#include <Cocktail/Graphic/Rendering/Queue/StaticMeshRecordInfo.hpp>

namespace Ck
{
    /**
     * \brief
     */
    struct InstancedStaticMeshRecordInfo
    {
        /**
         * \brief
         */
        StaticMeshRecordInfo StaticMeshRecord;

        /**
         * \brief
         */
        const Renderer::Buffer* InstancesBuffer = nullptr;

        /**
         * \brief
         */
        std::size_t InstanceBufferOffset = 0;

        /**
         * \brief
         */
        unsigned int InstanceCount = 1;

        /**
         * \brief
         */
        unsigned int FirstInstance = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_INSTANCEDSTATICMESHRECORDINFO_HPP
