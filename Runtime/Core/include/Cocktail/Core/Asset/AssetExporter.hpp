#ifndef COCKTAIL_CORE_ASSET_ASSETEXPORTER_HPP
#define COCKTAIL_CORE_ASSET_ASSETEXPORTER_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/Path.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
	/**
	 * \brief Base interface for all asset exporters
	 *
	 * Asset exporters are responsible for serializing assets of type \c T into
	 * various output formats such as files, memory buffers, and data streams.
	 * They are registered inside an \c AssetLoader to extend export capabilities.
	 *
	 * \tparam T The asset type handled by this exporter
	 * \tparam P Export parameters used to configure the serialization process
	 */
	template <typename T, typename P>
	class AssetExporter
	{
	public:

		/**
		 * \brief Default destructor
		 */
		virtual ~AssetExporter() = default;

		/**
		 * \brief Saves an asset to a file path
		 *
		 * Implementations must ensure the asset is correctly serialized to the
		 * specified path according to the applied export parameters.
		 *
		 * \param asset The asset instance to export
		 * \param path The destination file path
		 * \param parameters Export parameters used to configure the export
		 */
		virtual void SaveToPath(const T& asset, const Path& path, const P& parameters = {}) const = 0;

		/**
		 * \brief Saves an asset to an output stream.
		 *
		 * \param asset The asset to serialize.
		 * \param outputStream Output stream to write serialized data into.
	     * \param parameters Export parameters used to configure the export
		 */
		virtual void SaveToStream(const T& asset, OutputStream& outputStream, const P& parameters = {}) const = 0;

		/**
		 * \brief Serializes an asset into a byte array in memory
		 *
		 * \param asset The asset to export
		 * \param parameters Optional export parameters
		 *
		 * \return A ByteArray containing the serialized asset content
		 */
		virtual ByteArray SaveToMemory(const T& asset, const P& parameters = {}) const = 0;

		/**
		 * \brief Checks whether this exporter supports exporting a given file extension
		 *
		 * Extensions are typically compared case-insensitively and without leading dots,
		 * depending on implementation requirements.
		 *
		 * \param extension The file extension to test (e.g., "png", "txt")
		 *
		 * \return True if this exporter can export assets with the given extension
		 */
		virtual bool SupportExtension(StringView extension) const = 0;

		/**
		 * \brief Indicates whether the exporter supports a specific parameter configuration
		 *
		 * This can be used to validate feature sets such as format versions,
		 * compression settings, quality levels, etc.
		 *
		 * \param parameters The export parameters to evaluate
		 *
		 * \return True if the parameters are supported
		 */
		virtual bool SupportParameters(const P& parameters) const = 0;
	};
}

#endif // COCKTAIL_CORE_ASSET_ASSETEXPORTER_HPP
