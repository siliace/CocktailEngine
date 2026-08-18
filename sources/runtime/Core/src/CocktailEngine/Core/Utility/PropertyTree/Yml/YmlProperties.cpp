#include <cassert>
#include <cstdlib>
#include <cstring>

// The static inline helpers of cyaml.h (cyaml_val, cyaml_comment_at) return C99 compound
// literals such as `(cyaml_span_t){ 0 }`. GCC and Clang accept them in C++ as an extension,
// MSVC reports C4576 at error level, so the diagnostic is silenced for that header only.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4576)
#endif
#include <cyaml.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/Utility/Encoding/Encoders.hpp>
#include <CocktailEngine/Core/Utility/FileUtils.hpp>
#include <CocktailEngine/Core/Utility/PropertyTree/Yml/YmlNullNode.hpp>
#include <CocktailEngine/Core/Utility/PropertyTree/Yml/YmlProperties.hpp>

namespace Ck
{
    namespace
    {
        /**
         * \brief Encoding of the utf-8 byte order mark
         */
        constexpr Utf8Char ByteOrderMark[] = { 0xEF, 0xBB, 0xBF };

        /**
         * \brief Deleter releasing a document allocated by cyaml
         */
        struct CyamlDocumentDeleter
        {
            void operator()(cyaml_doc_t* document) const noexcept
            {
                cyaml_free(document);
            }
        };

        /**
         * \brief Alias of an owning pointer over a cyaml document
         */
        using CyamlDocument = UniquePtr<cyaml_doc_t, CyamlDocumentDeleter>;

        /**
         * \brief Owner of a scalar string allocated by cyaml
         *
         * cyaml_scalar_str allocates the string it returns, it is the responsibility of the caller to
         * release it. This holder does it, including when the conversion to a String throws.
         */
        class CyamlScalarString
        {
        public:

            /**
             * \brief Constructor
             * \param document The document holding the \p node
             * \param node The node to read the scalar value from
             */
            CyamlScalarString(const cyaml_doc_t* document, const cyaml_node_t* node) :
                mValue(cyaml_scalar_str(document, node))
            {
                /// Nothing
            }

            /**
             * \brief Copy constructor
             * \param other The instance to copy
             */
            CyamlScalarString(const CyamlScalarString& other) = delete;

            /**
             * \brief Destructor
             */
            ~CyamlScalarString()
            {
                std::free(mValue);
            }

            /**
             * \brief Copy assignment operator
             * \param other The instance to copy
             * \return A reference to this instance
             */
            CyamlScalarString& operator=(const CyamlScalarString& other) = delete;

            /**
             * \brief Check whether cyaml did return a string
             *
             * Only nodes which are not scalar have no string representation, a scalar holding an empty
             * value still returns an empty string.
             *
             * \return True if a string is held, false otherwise
             */
            bool IsValid() const
            {
                return mValue != nullptr;
            }

            /**
             * \brief Get the held string
             * \return The null terminated utf-8 string, or nullptr if there is none
             */
            const Utf8Char* Get() const
            {
                return reinterpret_cast<const Utf8Char*>(mValue);
            }

        private:

            char* mValue;
        };

        /**
         * \brief Context shared by the functions converting a Yml document into a property tree
         */
        struct YmlContext
        {
            /**
             * \brief The document being converted
             */
            const cyaml_doc_t* Document = nullptr;

            /**
             * \brief The name of the source of the document, reported by the exception messages
             */
            String Source;
        };

        /**
         * \brief Get a human readable name of the type of a node
         * \param node The node to name the type of
         * \return The name of the type of the \p node
         */
        const TextChar* GetNodeTypeName(const cyaml_node_t* node)
        {
            if (!node)
                return CK_TEXT("empty");

            switch (node->type)
            {
                case CYAML_NULL: return CK_TEXT("null");
                case CYAML_SCALAR: return CK_TEXT("scalar");
                case CYAML_SEQ: return CK_TEXT("sequence");
                case CYAML_MAP: return CK_TEXT("mapping");
                case CYAML_ALIAS: return CK_TEXT("alias");
                case CYAML_NONE:
                default: return CK_TEXT("unknown");
            }
        }

        /**
         * \brief Format the location of a node in its source, to be used as a prefix of an error message
         * \param context The conversion context
         * \param node The node to locate, might be nullptr to locate the source itself
         * \return The formatted location
         */
        String FormatLocation(const YmlContext& context, const cyaml_node_t* node)
        {
            if (!node)
                return context.Source;

            return String::Format(CK_TEXT("%s(%u,%u)"), context.Source, node->span.start_line, node->span.start_col);
        }

        /**
         * \brief Read the value of a scalar node as a String
         * \param context The conversion context
         * \param node The node to read
         * \return The value held by the \p node
         */
        String ReadScalarValue(const YmlContext& context, const cyaml_node_t* node)
        {
            if (!cyaml_is_scalar(node))
                throw YmlParseException(CK_TEXT("%s: expected a scalar node, got a %s node"), FormatLocation(context, node), GetNodeTypeName(node));

            CyamlScalarString scalar(context.Document, node);
            if (!scalar.IsValid())
                return String::Empty;

            return String::ConvertFrom<Encoders::Utf8>(scalar.Get());
        }

        void ProcessYmlNode(YmlProperties::SequenceType& sequence, const YmlContext& context, const cyaml_node_t* node);
        void ProcessYmlNode(YmlProperties::ElementType& element, const YmlContext& context, const String& name, const cyaml_node_t* node);

        YmlProperties::ValueType ProcessYmlScalarNode(const YmlContext& context, const cyaml_node_t* node)
        {
            assert(cyaml_is_scalar(node));

            return YmlProperties::ValueType(ReadScalarValue(context, node));
        }

        YmlProperties::SequenceType ProcessYmlSequenceNode(const YmlContext& context, const cyaml_node_t* node)
        {
            assert(cyaml_is_seq(node));

            YmlProperties::SequenceType sequence;
            for (Uint32 i = 0; i < cyaml_seq_len(node); i++)
                ProcessYmlNode(sequence, context, cyaml_seq_get(node, i));

            return sequence;
        }

        YmlProperties::ElementType ProcessYmlMapNode(const YmlContext& context, const cyaml_node_t* node)
        {
            assert(cyaml_is_map(node));

            YmlProperties::ElementType element;
            for (Uint32 i = 0; i < cyaml_map_len(node); i++)
            {
                const cyaml_pair_t* mapEntry = cyaml_map_at(node, i);

                // The property tree addresses the children of an element by name, a key which is not a
                // scalar has none. cyaml_scalar_str would return nullptr for such a key.
                if (!cyaml_is_scalar(mapEntry->key))
                    throw YmlParseException(CK_TEXT("%s: unsupported %s key, a mapping key must be a scalar"), FormatLocation(context, mapEntry->key),
                                            GetNodeTypeName(mapEntry->key));

                String name = ReadScalarValue(context, mapEntry->key);

                // Duplicated keys are reported by the parser as as many entries, and inserting a child
                // whose name is already taken drops the previous one: refuse the document instead of
                // silently keeping only the last entry.
                if (element.HasChild(name))
                    throw YmlParseException(CK_TEXT("%s: duplicated key %s in mapping"), FormatLocation(context, mapEntry->key), name);

                ProcessYmlNode(element, context, name, mapEntry->val);
            }

            return element;
        }

        void ProcessYmlNode(YmlProperties::ElementType& element, const YmlContext& context, const String& name, const cyaml_node_t* node)
        {
            if (cyaml_is_scalar(node))
            {
                element.Insert(name, ProcessYmlScalarNode(context, node));
            }
            else if (cyaml_is_seq(node))
            {
                element.Insert(name, ProcessYmlSequenceNode(context, node));
            }
            else if (cyaml_is_map(node))
            {
                element.Insert(name, ProcessYmlMapNode(context, node));
            }
            else if (cyaml_is_null(node))
            {
                // A key holding no value at all ("key:", "key: ~", "key: null") is a key of the mapping
                // nonetheless: bind it to a null node rather than dropping it from the tree
                element.Insert(name, YmlNullNode());
            }
            else
            {
                throw YmlParseException(CK_TEXT("%s: unsupported %s node for the key %s"), FormatLocation(context, node), GetNodeTypeName(node), name);
            }
        }

        void ProcessYmlNode(YmlProperties::SequenceType& sequence, const YmlContext& context, const cyaml_node_t* node)
        {
            if (cyaml_is_scalar(node))
            {
                sequence.Add(ProcessYmlScalarNode(context, node));
            }
            else if (cyaml_is_seq(node))
            {
                sequence.Add(ProcessYmlSequenceNode(context, node));
            }
            else if (cyaml_is_map(node))
            {
                sequence.Add(ProcessYmlMapNode(context, node));
            }
            else if (cyaml_is_null(node))
            {
                // Dropping an empty item would shift the index of every following one
                sequence.Add(YmlNullNode());
            }
            else
            {
                throw YmlParseException(CK_TEXT("%s: unsupported %s node at the index %u of a sequence"), FormatLocation(context, node), GetNodeTypeName(node), sequence.GetSize());
            }
        }
    }

    YmlProperties::YmlProperties(const Path& path)
    {
        YmlContext context;
        context.Source = path.ToString();

        Utf8String yml = FileUtils::ReadFileText<Encoders::Utf8>(path);

        const Utf8Char* content = yml.GetData();
        unsigned int length = yml.GetLength();

        // A byte order mark is a valid prefix of a Yml stream, but the parser reads it as a part of the
        // first scalar of the document, where it would end up in the name of the first key
        if (length >= sizeof(ByteOrderMark) && std::memcmp(content, ByteOrderMark, sizeof(ByteOrderMark)) == 0)
        {
            content += sizeof(ByteOrderMark);
            length -= sizeof(ByteOrderMark);
        }

        // GetData returns nullptr on an empty string, which cyaml_parse reports as a syntax error
        if (length == 0)
        {
            ResetRoot(String::Empty, ElementType());
            return;
        }

        cyaml_error_t error{};
        CyamlDocument document(cyaml_parse(reinterpret_cast<const char*>(content), length, nullptr, &error));

        if (!document)
        {
            // A source holding no document at all, comments and blank lines only, is reported as a
            // failure without an error code rather than as an empty document
            if (error.code != CYAML_OK)
            {
                throw YmlParseException(CK_TEXT("%s(%u,%u): %s"), context.Source, error.span.start_line, error.span.start_col, error.msg);
            }

            ResetRoot(String::Empty, ElementType());
            return;
        }

        context.Document = document.Get();

        // The parser leaves the aliases unresolved, and the property tree has no way to represent them:
        // replace each of them by a copy of the node its anchor names. An alias naming an unknown anchor
        // is not rejected by the parser, it is left unresolved and reported here.
        if (!cyaml_resolve_aliases(document.Get()))
        {
            throw YmlParseException(CK_TEXT("%s: the document holds an alias naming an unknown anchor"), context.Source);
        }

        const cyaml_node_t* root = cyaml_root(document.Get());

        // A document holding nothing but a null value is an empty tree
        if (cyaml_is_null(root))
        {
            ResetRoot(String::Empty, ElementType());
            return;
        }

        // The root of a property tree is an element, so the root of the document has to be a mapping
        if (!cyaml_is_map(root))
        {
            throw YmlParseException(CK_TEXT("%s: the root of the document is a %s node, a mapping is expected"), context.Source, GetNodeTypeName(root));
        }

        ResetRoot(String::Empty, ProcessYmlMapNode(context, root));
    }
}
