#ifndef COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP
#define COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP

#include <filesystem>

#include <Cocktail/Core/IO/Input/Stream/BufferedInputStream.hpp>
#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>

namespace Ck
{
	/**
	 * \brief Base interface for all asset importers
	 *
	 * Asset importers are responsible for deserializing assets of type \c T from
	 * various input sources, including files, streams, and memory buffers.
	 * They are used by \c AssetLoader to extend supported input formats.
	 *
	 * \tparam T The asset type produced by the importer
	 * \tparam P Import parameters used to configure the loading process
	 */
	template <typename T, typename P>
	class AssetImporter
	{
	public:

		/**
		 * \brief Default destructor
		 */
		virtual ~AssetImporter() = default;

		/**
		 * \brief Loads an asset from a file path
		 *
		 * Opens the file as a stream and delegates the actual loading
		 * to \c LoadFromStream() implemented by derived classes.
		 *
		 * \param path Path of the asset to read
		 * \param parameters Import parameters used to configure the import
		 *
		 * \return A shared pointer to the loaded asset, or nullptr if loading fails
		 */
		virtual std::shared_ptr<T> LoadFromPath(const Path& path, const P& parameters = {})
		{
			UniquePtr<File> file = Storage::OpenFile(path, FileOpenFlagBits::Read);
			FileInputStream inputStream(*file);
			BufferedInputStream bufferedInputStream(inputStream);

			return LoadFromStream(bufferedInputStream, parameters);
		}

		/**
		 * \brief Loads an asset from an input stream
		 *
		 * Derived classes must implement the logic for reading and decoding asset data
		 * from the provided stream based on the given import parameters.
		 *
		 * \param inputStream Stream providing serialized asset data
		 * \param parameters Import parameters used to configure the import
		 *
		 * \return A shared pointer to the loaded asset, or nullptr on failure
		 */
		virtual std::shared_ptr<T> LoadFromStream(InputStream<>& inputStream, const P& parameters = {}) = 0;

		/**
		 * \brief Loads an asset from a raw memory buffer
		 *
		 * Wraps the buffer in a \c MemoryInputStream and calls \c LoadFromStream().
		 *
		 * \param buffer Pointer to the memory region containing asset data
		 * \param length Size of the data in bytes
		 * \param parameters Import parameters used to configure the import
		 *
		 * \return A shared pointer to the loaded asset, or nullptr if loading fails.
		 */
		virtual std::shared_ptr<T> LoadFromMemory(const Byte* buffer, std::size_t length, const P& parameters = {})
		{
			MemoryInputStream inputStream(buffer, length);
			return LoadFromStream(inputStream, parameters);
		}

		/**
		 * \brief Indicates whether this importer supports a particular file extension
		 *
		 * \param extension File extension to check (e.g., "png", "obj")
		 *
		 * \return True if the importer can load files with this extension
		 */
		virtual bool SupportExtension(StringView extension) const = 0;

		/**
		 * \brief Checks whether the importer supports a specific parameter configuration
		 *
		 * Use this to validate compatibility with desired decoding settings,
		 * compression modes, versioning, and so on.
		 *
		 * \param parameters The parameters to validate
		 *
		 * \return True if the parameters are supported by the importer
		 */
		virtual bool SupportParameters(const P& parameters) const = 0;
	};
}

#endif // COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP
