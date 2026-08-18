#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/DataType.hpp>
#include <CocktailEngine/Core/Flags.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>

#include <CocktailEngine/Graphic/Geometry/Vertex/VertexAttribute.hpp>

namespace Ck
{
    /**
     * \brief Describes the configuration used to create a vertex layout
     */
    struct VertexLayoutCreateInfo
    {
        /**
         * \brief Describes a vertex attribute
         */
        struct VertexLayoutAttributeInfo
        {
            /**
             * \brief The semantic identifying the attribute
             */
            VertexAttributeSemantic Semantic;

            /**
             * \brief The data type of each attribute element
             */
            DataType Type;

            /**
             * \brief The number of elements in the attribute
             */
            unsigned int ElementCount = 0;

            /**
             * \brief The number of consecutive attribute values stored per vertex
             */
            unsigned int ArrayLength = 1;

            /**
             * \brief Whether attribute values must be normalized when interpreted
             */
            bool Normalized = false;
        };

        /**
         * \brief The number of vertex attributes defined
         */
        unsigned int VertexAttributeCount = 0;

        /**
         * \brief The vertex attributes defining the layout
         */
        VertexLayoutAttributeInfo VertexAttributes[Enum<VertexAttributeSemantic>::ValueCount];

        /**
         * \brief Whether the layout is used for instanced rendering
         */
        bool Instanced = false;

        /**
         * \brief The number of instances advanced between consecutive attribute updates
         */
        unsigned int Divisor = 0;
    };

    /**
     * \brief Describes the format and input configuration of vertex data
     */
    class COCKTAILENGINE_GRAPHIC_API VertexLayout
    {
    public:

        /**
         * \brief Builder for configuring and creating a vertex layout
         */
        class COCKTAILENGINE_GRAPHIC_API Builder
        {
        public:

            /**
             * \brief Add an attribute to the vertex layout
             * \param semantic The semantic identifying the attribute
             * \param dataType The data type of each attribute element
             * \param elementCount The number of elements in the attribute
             * \param arrayLength The number of consecutive attribute values stored per vertex
             * \param normalized Whether attribute values must be normalized when interpreted
             * \return A reference to this builder
             */
            Builder& AddAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength = 1, bool normalized = false);

            /**
             * \brief Set whether the layout is used for instanced rendering
             * \param instanced True if the layout is used for instanced rendering, false otherwise
             * \return A reference to this builder
             */
            Builder& SetInstanced(bool instanced = true);

            /**
             * \brief Set the instance divisor for the layout
             * \param divisor The number of instances between consecutive attribute updates
             * \return A reference to this builder
             */
            Builder& SetDivisor(unsigned int divisor);

            /**
             * \brief Create a vertex layout from the current configuration
             * \return A shared pointer to the created vertex layout
             */
            SharedPtr<VertexLayout> Get();

            /**
             * \brief Get the configuration used by this builder
             * \return The vertex layout creation information
             */
            const VertexLayoutCreateInfo& ToCreateInfo() const;

        private:

            VertexLayoutCreateInfo mCreateInfo;
        };

        /**
         * \brief Constructor
         *
         * \param createInfo The configuration used to create the vertex layout
         */
        explicit VertexLayout(const VertexLayoutCreateInfo& createInfo);

        /**
         * \brief Find an attribute by its semantic
         *
         * \param semantic The semantic identifying the attribute
         *
         * \return A pointer to the matching attribute, or nullptr if no matching attribute exists
         */
        const VertexAttribute* FindAttribute(VertexAttributeSemantic semantic) const;

        /**
         * \brief Check whether this layout is compatible with another layout
         *
         * \param other The vertex layout to compare with
         *
         * \return True if both layouts are compatible, false otherwise
         */
        bool IsCompatibleWith(const VertexLayout& other) const;

        /**
         * \brief Get the size in bytes of a vertex
         *
         * \return The vertex stride in bytes
         */
        unsigned int GetStride() const;

        /**
         * \brief Get the mask of semantics used by this layout
         *
         * \return The mask of vertex attribute semantics
         */
        Flags<VertexAttributeSemantic> GetUsageMask() const;

        /**
         * \brief Check whether the layout is used for instanced rendering
         *
         * \return True if the layout is used for instanced rendering, false otherwise
         */
        bool IsInstanced() const;

        /**
         * \brief Get the instance divisor
         *
         * \return The number of instances between consecutive attribute updates
         */
        unsigned int GetDivisor() const;

        /**
         * \brief Get the attributes defining the vertex layout
         *
         * \return The array of vertex attributes
         */
        const Array<VertexAttribute>& GetAttributes() const;

        /**
         * \brief Check whether two vertex layouts are equal
         *
         * \param rhs The vertex layout to compare with
         *
         * \return True if both layouts are equal, false otherwise
         */
        bool operator==(const VertexLayout& rhs) const;

        /**
         * \brief Check whether two vertex layouts are different
         *
         * \param rhs The vertex layout to compare with
         *
         * \return True if both layouts are different, false otherwise
         */
        bool operator!=(const VertexLayout& rhs) const;

    private:

        /**
         * \brief Create a vertex attribute
         *
         * \param semantic The semantic identifying the attribute
         * \param dataType The data type of each attribute element
         * \param elementCount The number of elements in the attribute
         * \param arrayLength The number of consecutive attribute values stored per vertex
         * \param normalized Whether attribute values must be normalized when interpreted
         *
         * \return A reference to the created attribute
         */
        const VertexAttribute& CreateAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength = 1,
                                               bool normalized = false);

        bool mInstanced;
        unsigned int mDivisor;
        unsigned int mStride;
        Flags<VertexAttributeSemantic> mUsageMask;
        Array<VertexAttribute> mAttributes;
    };
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP
