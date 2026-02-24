#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMINTERFACE_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMINTERFACE_HPP

#include <Cocktail/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp>
#include <Cocktail/Graphic/Material/Material.hpp>

#include <Cocktail/Renderer/Shader/DescriptorType.hpp>
#include <Cocktail/Renderer/Texture/StaticSampler.hpp>

namespace Ck
{
    /**
     * \brief A type representing a binding slot for a shader resource
     *
     * A binding slot identifies a specific resource (buffer, texture, sampler) that can be accessed from a shader
     * within a binding domain.
     */
    using BindingSlot = unsigned int;

    /**
     * \brief Represents an invalid binding slot
     *
     * Used to indicate that a binding slot is uninitialized or not set.
     */
    static constexpr BindingSlot InvalidBindingSlot = -1;

    /**
     * \brief Shader binding domains grouping related resources
     *
     * Each domain defines a set of binding slots that are logically grouped together for the shader.
     */
    enum class ShaderBindingDomain
    {
        Scene, /*!< Resources shared across the entire scene, like global lighting or scene info */
        Viewport, /*!< Resources specific to the viewport or camera, like view/projection matrices */
        Material, /*!< Resources specific to a material, like textures and material properties */
        Drawcall, /*!< Resources specific to a single draw call, like per-instance data */
    };

    /**
     * \brief Binding slots for the scene domain
     *
     * Provides constants for accessing common scene-level resources.
     */
    class SceneBindingSlots
    {
    public:

        static constexpr BindingSlot SceneInfo = 0; /*!< Slot containing global scene information */
    };

    /**
     * \brief Binding slots for the viewport domain
     *
     * Provides constants for accessing viewport-specific resources
     */
    class ViewportBindingSlots
    {
    public:

        static constexpr BindingSlot CameraInfo = 0; /*!< Slot containing camera matrices and parameters */
        static constexpr BindingSlot Lights = 1; /*!< Slot containing light data */
    };

    /**
     * \brief Binding slots for the material domain
     *
     * Provides constants for accessing material-specific resources used by shaders
     */
    class MaterialBindingSlots
    {
    public:

        static constexpr BindingSlot BaseColor = 0; /*!< Slot containing base color texture */
        static constexpr BindingSlot MetallicRoughness = 1; /*!< Slot containing metallic/roughness texture */
        static constexpr BindingSlot Normal = 2; /*!< Slot containing normal map */
        static constexpr BindingSlot Alpha = 3; /*!< Slot containing alpha map */
        static constexpr BindingSlot Emission = 4; /*!< Slot containing emission texture */
    };

    /**
     * \brief Binding slots for the drawcall domain
     *
     * Provides constants for accessing drawcall-specific resources
     */
    class DrawcallBindingSlots
    {
    public:

        static constexpr BindingSlot Instances = 0; /*!< Slot containing per-instance data */
    };

    /**
     * \brief Describes a single slot exposed by a binding domain
     *
     * Includes the resource name, type, and optionally a static sampler
     */
    struct SlotInterface
    {
        AsciiString Name; /*!< Name of the slot */
        Renderer::DescriptorType Type; /*!< Type of the resource (texture, buffer, etc.) */
        Renderer::StaticSampler Sampler = Renderer::StaticSampler::None; /*!< Optional static sampler */
    };

    /**
     * \brief Interface of a binding domain, managing multiple binding slots
     *
     * A BindingDomainInterface allows setting and querying the slots available for a specific domain.
     * Each domain may contain multiple slots that shaders can bind to.
     */
    class COCKTAIL_GRAPHIC_API BindingDomainInterface
    {
    public:

        /**
         * \brief Get the index of a binding slot within the domain interface
         *
         * \param bindingSlot The binding slot to query
         *
         * \return Optional index of the slot interface if it exists
         */
        Optional<unsigned int> GetSlotInterfaceIndex(BindingSlot bindingSlot) const;

        /**
         * \brief Get the number of slot interfaces exposed by this domain
         *
         * \return The total number of slots in this domain
         */
        unsigned int GetSlotInterfaceCount() const;

        /**
         * \brief Get the slot interface at a given index
         *
         * \param index Index of the slot interface
         *
         * \return Reference to the slot interface
         */
        const SlotInterface& GetSlotInterface(unsigned int index) const;

        /**
         * \brief Define or modify a binding slot in the domain interface
         *
         * \param bindingSlot The slot to set
         * \param name Name of the slot
         * \param type Type of the resource
         * \param sampler Optional static sampler
         */
        void SetSlot(BindingSlot bindingSlot, AsciiString name, Renderer::DescriptorType type, Renderer::StaticSampler sampler);

    private:

        /**
         * \brief Internal representation of a binding slot with its interface
         */
        struct BindingSlotInterface
        {
            BindingSlot BindingSlot = InvalidBindingSlot; /*!< The binding slot index */
            SlotInterface Interface; /*!< The interface data for this slot */
        };

        Array<BindingSlotInterface> mBindingSlotInterfaces; /*!< All slot interfaces in this domain */
    };

    /**
     * \brief Describes a vertex attribute exposed by a material program
     *
     * Used to map vertex data semantics to names accessible in the shader
     */
    struct VertexInterface
    {
        AsciiString Name; /*!< Name of the vertex attribute */
    };

    /**
     * \brief Interface of a material program, including vertex attributes and binding domains
     *
     * MaterialProgramInterface defines which vertex attributes and shader resources are expected by a material shader.
     * It organizes resources into domains and allows querying slots and attributes at runtime
     */
    class COCKTAIL_GRAPHIC_API MaterialProgramInterface
    {
    public:

        /**
         * \brief Builder to construct a MaterialProgramInterface
         *
         * Provides a convenient way to define vertex attributes and binding slots before finalizing the interface
         */
        class Builder
        {
        public:

            Builder();

            /**
             * \brief Set a vertex attribute name
             *
             * \param attribute The vertex attribute semantic
             * \param name The name to assign to the attribute
             *
             * \return Reference to the builder for chaining
             */
            Builder& SetVertexAttribute(VertexAttributeSemantic attribute, AsciiString name);

            /**
             * \brief Set a binding slot in a specific domain
             *
             * \param domain The shader binding domain
             * \param bindingSlot The binding slot index
             * \param name Name of the slot
             * \param type Resource type
             * \param sampler Optional static sampler
             *
             * \return Reference to the builder for chaining
             */
            Builder& SetBindingSlot(ShaderBindingDomain domain, BindingSlot bindingSlot, AsciiString name, Renderer::DescriptorType type, Renderer::StaticSampler sampler = Renderer::StaticSampler::None);

            /**
             * \brief Finalize and get the MaterialProgramInterface
             * *
             * \return Shared pointer to the constructed interface
             */
            std::shared_ptr<MaterialProgramInterface> Get() const;

        private:

            std::shared_ptr<MaterialProgramInterface> mInterface; /*!< Interface being built */
        };

        MaterialProgramInterface() = default;
        MaterialProgramInterface(const MaterialProgramInterface& other) = delete;
        MaterialProgramInterface(MaterialProgramInterface&& other) = default;
        MaterialProgramInterface& operator=(const MaterialProgramInterface& other) = delete;
        MaterialProgramInterface& operator=(MaterialProgramInterface&& other) = default;

        /**
         * \brief Get the vertex interface for a given semantic
         *
         * \param semantic The vertex attribute semantic
         *
         * \return Pointer to the vertex interface if present, nullptr otherwise
         */
        const VertexInterface* GetVertexInterface(VertexAttributeSemantic semantic);

        /**
         * \brief Get the binding domain interface for a given domain
         *
         * \param domain The shader binding domain
         *
         * \return Pointer to the domain interface if present, nullptr otherwise
         */
        const BindingDomainInterface* GetBindingDomainInterface(ShaderBindingDomain domain) const;

    private:

        std::unordered_map<VertexAttributeSemantic, VertexInterface> mVertexAttributes; /*!< Vertex attributes */
        EnumMap<ShaderBindingDomain, UniquePtr<BindingDomainInterface>> mDomainInterfaces; /*!< Domain interfaces */
    };
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMINTERFACE_HPP
