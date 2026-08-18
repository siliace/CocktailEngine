#ifndef COCKTAILENGINE_CORE_UTILITY_FINDER_HPP
#define COCKTAILENGINE_CORE_UTILITY_FINDER_HPP

#include <functional>
#include <limits>

#include <CocktailEngine/Core/System/FileSystem/Local/LocalFileSystem.hpp>
#include <CocktailEngine/Core/System/FileSystem/Path.hpp>

namespace Ck
{
    /**
     * \brief Collects entries from one or more directories
     *
     * Finder is a stateful, fluent query builder for directory contents. Sources and
     * filters configured on an instance are retained between calls to Get(), allowing
     * a query to be assembled through chained calls. Results retain the traversal
     * order supplied by the filesystem driver; Finder neither sorts nor deduplicates
     * them.
     *
     * By default, Finder returns the immediate files and directories of every source.
     * Recursion, file-extension filtering and the inclusion of files or directories
     * can be configured independently.
     *
     * \remark Finder is not thread safe. A single instance must not be configured or
     *         queried concurrently from multiple threads.
     */
    class COCKTAILENGINE_CORE_API Finder
    {
    public:

        /**
         * \brief Predicate used to decide whether an entry is returned
         *
         * The predicate receives the path as it would appear in Get()'s result.
         */
        using Predicate = std::function<bool(const Path&)>;

        /**
         * \brief Adds a directory to search
         *
         * Sources are appended rather than replaced, and their matching entries are
         * appended to the result in the same order in which the sources were added.
         *
         * \param path Directory to search
         *
         * \return This finder, to continue configuring the query
         */
        Finder& From(const Path& path);

        /**
         * \brief Restricts matching files to an extension
         *
         * A non-empty extension without a leading dot is normalized by adding one.
         * Passing an empty string removes the extension filter. The filter is applied
         * to files only and compared exactly, including its case.
         *
         * \param fileExtension Extension to match, with or without a leading dot
         *
         * \return This finder, to continue configuring the query
         */
        Finder& WithExtension(String fileExtension);

        /**
         * \brief Adds a predicate that an entry must satisfy to be returned
         *
         * Predicates added by successive calls are combined with a logical AND. A
         * rejected directory is hidden from the result, but is still traversed by a
         * recursive query unless it was excluded with ExcludeDirectory. Empty
         * predicates are ignored.
         *
         * \param predicate Predicate evaluated for every candidate entry
         *
         * \return This finder, to continue configuring the query
         */
        Finder& Where(Predicate predicate);

        /**
         * \brief Excludes an exact path from the returned entries
         *
         * The path is compared in the logical namespace of the configured sources.
         * Excluding a directory only hides that directory itself; recursive queries
         * still traverse it. Use ExcludeDirectory to skip an entire subtree.
         *
         * \param path File or directory path to hide from the results
         *
         * \return This finder, to continue configuring the query
         */
        Finder& Exclude(const Path& path);

        /**
         * \brief Excludes a directory and all of its descendants
         *
         * A matching directory is neither returned nor opened during recursive
         * traversal. A source equal to, or contained in, an excluded directory is
         * skipped as well.
         *
         * \param path Root of the directory subtree to skip
         *
         * \return This finder, to continue configuring the query
         */
        Finder& ExcludeDirectory(const Path& path);

        /**
         * \brief Includes or excludes files from the results
         *
         * Excluding files does not affect directory traversal, so recursive searches
         * can still discover directories and their descendants.
         *
         * \param ignoreFiles Whether files should be excluded; true by default
         *
         * \return This finder, to continue configuring the query
         */
        Finder& IgnoreFiles(bool ignoreFiles = true);

        /**
         * \brief Includes or excludes directories from the results
         *
         * Excluding directories only hides them from the returned array. Recursive
         * searches still enter them while a positive recursion depth remains.
         *
         * \param ignoreDirectories Whether directories should be excluded; true by default
         *
         * \return This finder, to continue configuring the query
         */
        Finder& IgnoreDirectories(bool ignoreDirectories = true);

        /**
         * \brief Configures recursive traversal
         *
         * Disabling recursion sets the traversal depth to zero. A depth of zero still
         * inspects the direct contents of each source, but never enters a child
         * directory. A depth of one enters direct child directories, and so on.
         *
         * \param recursive Whether child directories should be explored
         * \param depth Maximum number of directory levels to enter when \p recursive
         *              is true; unlimited by default
         *
         * \return This finder, to continue configuring the query
         */
        Finder& Recursively(bool recursive, unsigned int depth = std::numeric_limits<unsigned int>::max());

        /**
         * \brief Returns entries matching the configured query
         *
         * Every configured source is enumerated with \p driver. Filesystem errors,
         * including an invalid source or an unreadable directory, are propagated by
         * the driver. If no source was configured, an empty array is returned.
         *
         * \param driver Filesystem driver used to enumerate and classify entries;
         *               the local root driver is used by default
         *
         * \return Matching paths in source and filesystem traversal order
         */
        Array<Path> Get(FileSystemDriver& driver = LocalFileSystem::GetRootDriver()) const;

    private:

        /**
         * \brief Determines whether all configured predicates accept a path
         *
         * \param path Candidate path to evaluate
         *
         * \return Whether every predicate accepts \p path
         */
        bool Matches(const Path& path) const;

        /**
         * \brief Determines whether a path is hidden by an exact exclusion
         *
         * \param path Candidate path to test
         *
         * \return Whether \p path is excluded from the results
         */
        bool IsExcluded(const Path& path) const;

        /**
         * \brief Determines whether a path belongs to a skipped directory subtree
         *
         * \param path Candidate directory or source path to test
         *
         * \return Whether \p path is equal to or contained in an excluded directory
         */
        bool IsExcludedDirectory(const Path& path) const;

        /**
         * \brief Enumerates one source directory
         *
         * \param source Directory currently being enumerated
         * \param depth Remaining directory levels that may be entered
         * \param driver Filesystem driver used to enumerate and classify entries
         *
         * \return Entries matching the configured filters beneath \p source
         */
        Array<Path> Get(const Path& source, unsigned int depth, FileSystemDriver& driver) const;

        bool mIgnoreFiles = false;
        bool mIgnoreDirectories = false;
        unsigned int mDepth = 0;
        String mFileExtension;
        Array<Predicate> mPredicates;
        Array<Path> mExcluded;
        Array<Path> mExcludedDirectories;
        Array<Path> mSource;
    };

}
#endif // COCKTAILENGINE_CORE_UTILITY_FINDER_HPP
