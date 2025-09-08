#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_URI_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_URI_HPP

#include <Cocktail/Core/System/FileSystem/Path.hpp>

namespace Ck
{
    /**
     * \class URI
     * \brief Represents a Uniform Resource Identifier composed of a scheme and a filesystem path
     *
     * This class provides a lightweight URI abstraction where a scheme (also called protocol)
     * can be attached to a platform-independent filesystem path.
     *
     * Example formats supported:
     *   - file:/usr/local/bin
     *   - file:C:/Windows/System32
     *   - custom:relative/path
     *   - /usr/local/bin (no scheme, treated as a pure path)
     *
     * The parsing logic follows the simplified format:
     *
     *     <scheme> ":" <path>
     *
     * If the URI does not contain a valid scheme followed by a colon, the full string
     * is interpreted as a local filesystem path without scheme.
     *
     * \see Path for the internal path representation and platform specific behavior
     */
    class COCKTAIL_CORE_API URI
    {
    public:

	    /**
         * \brief 
         */
        static const StringView SchemeSeparator;

        /**
         * \brief Parses a URI string of the form "<scheme>:<path>"
         *
         * \param string Pointer to the URI string data
         *
         * \return A parsed URI instance
         */
	    static URI Parse(const String& string);

        /**
         * \brief Parses a URI string of the form "<scheme>:<path>"
         *
         * \param string Pointer to the URI string data
         *
         * \return A parsed URI instance
         */
	    static URI Parse(StringView string);

        /**
         * \brief Parses a URI string of the form "<scheme>:<path>"
         * 
         * \param string Pointer to the URI string data
         *
         * \return A parsed URI instance
         */
	    static URI Parse(const TextChar* string);

	    /**
         * \brief Parses a URI string of the form "<scheme>:<path>"
         *
         * The method extracts a valid scheme only if:
         *  - It is composed of alphanumeric characters, '+', '-' or '.'
         *  - It is followed by a colon ':'
         *
         * Otherwise, the entire input string is treated as a path only,
         * with an empty scheme.
         *
         * \param string Pointer to the URI string data
         * \param length Number of characters in the input string
         *
         * \return A parsed URI instance
         */
        static URI Parse(const TextChar* string, String::SizeType length);

        /**
         * \brief Default constructor
         * Create an empty URI with no scheme and an empty path
         */
        URI() = default;

        /**
         * \brief Constructor
         * Constructs a URI from a filesystem path without scheme
         *
         * \param path The platform-independent path component
         */
        URI(Path path);

        /**
         * \brief Constructor
         * Constructs a URI by parsing a string
         *
         * \param string The URI string data
         */
        URI(StringView string);

        /**
         * \brief Constructor
         * Constructs a URI by parsing a string
         *
         * \param string The URI string data
         */
        URI(const String& string);

        /**
         * \brief Constructor
         * Constructs a URI by parsing a null-terminated C-string
         *
         * \param string Pointer to the URI string data
         */
        URI(const TextChar* string);

        /**
         * \brief Constructor
         * Constructs a URI by parsing a C-string with explicit size
         *
         * \param string Pointer to the URI string data
         * \param length Number of characters in the input string
         */
        URI(const TextChar* string, String::SizeType length);

        /**
         * \brief Constructor
         * Constructs a URI from a scheme and a filesystem path
         *
         * \param scheme The URI scheme (e.g. "file", "custom")
         * \param path The platform-independent path component
         */
        URI(String scheme, Path path);

        /**
         * \brief Converts the URI back to a textual representation
         *
         * If a scheme is present, the output format is:
         *     <scheme>:<path>
         *
         * If no scheme exists, only the path is returned.
         *
         * \return A full URI string.
         */
        String ToString() const;

        /**
         * \brief Gets the scheme (protocol) part of the URI
         *
         * \return A string containing the scheme, or an empty string if none exists
         */
        const String& GetScheme() const;

        /**
         * \brief Gets the filesystem path part of the URI
         *
         * \return A reference to the internal Path object
         */
        const Path& GetPath() const;

        /**
         * \brief Compares two URIs for equality
         *
         * This operator compares the components of two URIs (scheme, authority, path, query, and fragment)
         * to determine equality according to the URI class's internal rules.
         *
         * \param other The URI to compare with
         *
         * \return true if the URI components match, false otherwise
         *
         * \note Two different URIs may still refer to the same underlying resource depending on
         *       the filesystem or storage backend implementation. For example, symbolic links,
         *       mount points, or custom URI schemes may cause distinct URIs to point to the same file.
         */
        bool operator==(const URI& other) const;

        /**
         * \brief Compares two URIs for inequality
         *
         * This is the logical negation of operator==(). It returns true if the URIs differ
         * in any of their normalized components.
         *
         * \param other The URI to compare with
         *
         * \return true if the URIs are not equal, false otherwise
         */
        bool operator!=(const URI& other) const;

    private:

        String mScheme; /*!< Scheme/Protocol string (e.g. "file"), empty if not specified */
        Path mPath; /*!< Filesystem path stored using the Path class */
    };

	template <>
	struct TranslatorBetween<URI, String>
	{
		using Type = ToStringTranslator<URI>;
	};
}

namespace std
{
	template <>
	struct COCKTAIL_CORE_API hash<Ck::URI>
	{
		size_t operator()(const Ck::URI& uri) const noexcept;
	};
}
#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_URI_HPP
