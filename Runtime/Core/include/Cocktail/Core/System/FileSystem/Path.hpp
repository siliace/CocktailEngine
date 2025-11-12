#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_PATH_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_PATH_HPP

#include <Cocktail/Core/Utility/LazyValue.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>

namespace Ck
{
    /**
     * \class Path
     * \brief Represents and manipulates filesystem-like paths
     *
     * This class provides utilities to parse, normalize, join and inspect
     * filesystem paths in a platform-independent manner. On Windows, the
     * separator is '\\', while on other systems it is '/'.
     */
    class COCKTAIL_CORE_API Path
    {
    public:

        using SizeType = String::SizeType;

        /**
         * \brief Enumeration of possible path formating
         * This will change the separator used by the path
         */
        enum class Format
        {
            /**
             * \brief Leave the separators as is in the path
             * This can leave to mixes between '/' and '\'.
             */
            Auto,

            /**
             * \brief Use '/' everywhere it is possible
             */
            Generic,

            /**
             * \brief Use the system separator
             * On Windows, this will be '\' and '/' on other unix style systems.
             */
            System,
        };

    #ifdef COCKTAIL_OS_WINDOWS
        /**
         * \brief Platform-dependent path separator
         *
         * \note On Windows this is '\\'
         */
        static constexpr TextChar SystemSeparator = CK_CHAR('\\');
    #else
        /**
         * \brief Platform-dependent path separator
         *
         * \note On POSIX-like systems this is '/'
         */
        static constexpr TextChar SystemSeparator = CK_CHAR('/');
    #endif

        /**
		 * \brief Utility constant value to return a reference/pointer to an empty Path
         */
        static const Path Empty;

        /**
         * \brief Parses a character buffer into a Path object
         *
         * \param string The path string data
         * \param format The format of the path to parse
         *
         * \return The parsed Path object
         */
        static Path Parse(const String& string, Format format = Format::Auto);

        /**
         * \brief Parses a character buffer into a Path object
         *
         * \param string The path string data
         * \param format The format of the path to parse
         *
         * \return The parsed Path object
         */
        static Path Parse(StringView string, Format format = Format::Auto);

        /**
         * \brief Parses a character buffer into a Path object
         *
         * \param path Pointer to the character buffer
         * \param format The format of the path to parse
         *
         * \return The parsed Path object
         */
        static Path Parse(const TextChar* path, Format format = Format::Auto);

        /**
         * \brief Parses a character buffer into a Path object
         *
         * \param path Pointer to the character buffer
         * \param length Number of characters to parse
         * \param format The format of the path to parse
         *
         * \return The parsed Path object
         */
        static Path Parse(const TextChar* path, SizeType length, Format format = Format::Auto);

        template <typename... Args>
        static Path Merge(Args&&... args)
        {
            Path p;
            (p.Join(std::forward<Args>(args)), ...);

            return p;
        }

        /**
         * \brief Tell whether a character is a path separator
         *
         * \param character The character to test
         *
         * \return True if \p character is a separator, false otherwise
         */
        static bool IsSeparator(TextChar character);

        /**
         * \brief Default constructor
         * Creates an empty path
         */
        Path() = default;

        /**
         * \brief Constructor
         * Constructs a path by parsing a string
         *
         * \param string The path string data
         * \param format The format of the path to parse
         */
        Path(StringView string, Format format = Format::Auto);

        /**
         * \brief Constructor
         * Constructs a path by parsing a string
         *
         * \param string The path string data
         * \param format The format of the path to parse
         */
        Path(const String& string, Format format = Format::Auto);

        /**
         * \brief Constructor
         * Constructs a path by parsing a null-terminated C-string
         *
         * \param string Null-terminated text representing the path
         * \param format The format of the path to parse
         */
        Path(const TextChar* string, Format format = Format::Auto);

        /**
         * \brief Constructor
         * Constructs a path by parsing a C-string with explicit size
         *
         * \param string Pointer to the path string data
         * \param length Number of characters in the input string
         * \param format The format of the path to parse
         */
        Path(const TextChar* string, SizeType length, Format format = Format::Auto);

        /**
         * \brief Assigns the path content from a string
         *
         * \param string The path string data
         *
         * \return Reference to this Path after assignment
         */
        Path& operator=(StringView string);

        /**
         * \brief Assigns the path content from a string
         *
         * \param string The path string data
         *
         * \return Reference to this Path after assignment
         */
        Path& operator=(const String& string);

        /**
         * \brief Joins another path element to this path
         *
         * \param other The path segment to append
         *
         * \return Reference to this Path after joining
         */
        Path& Join(StringView other);

        /**
         * \brief Joins another Path to this one
         *
         * \param other The other Path to append
         *
         * \return Reference to this Path after joining
         */
        Path& Join(const Path& other);

        /**
         * \brief Joins a String to the current path
         *
         * \param other The String to append as a path segment
         *
         * \return Reference to this Path after joining
         */
        Path& Join(const String& other);

        /**
         * \brief Joins a null-terminated C-string to the current path
         *
         * \param other Null-terminated text to append as a path segment
         *
         * \return Reference to this Path after joining
         */
        Path& Join(const TextChar* other);

        /**
         * \brief Joins a character buffer with explicit size to the current path
         *
         * \param other Pointer to the characters to append
         * \param length Number of characters to append
         *
         * \return Reference to this Path after joining
         */
        Path& Join(const TextChar* other, SizeType length);

        /**
         * \brief Checks whether the path is empty
         *
         * \return True if the path contains no root or elements; false otherwise
         */
        bool IsEmpty() const;

        /**
         * \brief Determines if the path is absolute
         *
         * \return True if the path contains a root component (is absolute); false for relative paths
         */
        bool IsAbsolute() const;

        /**
         * \brief Retrieves the root of this path
         * 
         * If this path is empty, String::Empty is returned
         * 
         * \return The root
         */
        const String& GetRoot() const;

        /**
         * \brief Retrieves the parent directory of this path
         *
         * \return A new Path representing the parent directory
         *
         * \note If the path is empty or has no parent, the root is returned
         */
        Path GetParent() const;

        /**
         * \brief Retrieves the filename component of this path
         *
         * \return A Path representing only the filename component.
         *
         * \note If the path ends with a separator or is empty, an empty path is returned
         */
        Path GetFilename() const;

        /**
         * \brief Retrieves the file extension of the path
         *
         * \return The extension (with the leading dot)
         */
        String GetExtension() const;

        /**
         * \brief Converts the path to a String representation
         *
         * \return The full path as a String, including root and separators
         */
        String ToString() const;

        /**
         * \brief Equality comparison operator
         *
         * \param rhs The Path to compare against
         *
         * \return True if both paths are equal (typically after normalization); false otherwise
         */
        bool operator==(const Path& rhs) const;

        /**
         * \brief Inequality comparison operator
         *
         * \param rhs The Path to compare against
         *
         * \return True if the paths are not equal; false if they are equal
         */
        bool operator!=(const Path& rhs) const;

    private:

        /**
         * \brief Internal constructor used to build a path from root and elements
         *
         * \param root String representing the root component (for example "/" or "C:\\")
         * \param elements Array of path element strings (segments)
         * \param format The format of the parsed path
         */
        Path(String root, Array<String> elements, Format format);

        String mRoot; /*!< Root component of the path (for example "/" or "C:\") */
        Array<String> mElements; /*!< Path elements stored as an array of strings */
        Format mFormat; /*!< The format of path used */
        LazyValue<String> mJoined; /*!< Cached joined string representation (lazy evaluation) */
    };

    template <>
    struct TranslatorBetween<Path, String>
    {
        using Type = ToStringTranslator<Path>;
    };
}

namespace std
{
    template <>
    struct hash<Ck::Path>
    {
        size_t operator()(const Ck::Path& path) const noexcept
        {
            return hash<Ck::String>()(path.ToString());
        }
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_PATH_HPP
