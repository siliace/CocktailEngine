#ifndef COCKTAIL_CORE_ASSET_ASSETLOADER_HPP
#define COCKTAIL_CORE_ASSET_ASSETLOADER_HPP

#include <Cocktail/Core/Asset/AssetExporter.hpp>
#include <Cocktail/Core/Asset/AssetImporter.hpp>

namespace Ck
{
	/**
	 * \brief Generic asset loader handling both import and export of asset types
	 *
	 * This class provides a unified interface to load and save assets of type \c T
	 * from various sources such as file paths, memory buffers, and streams.
	 * It relies on registered \c AssetImporter and \c AssetExporter instances
	 * to support different formats and parameter configurations.
	 *
	 * \tparam T           The asset type handled by this loader
	 * \tparam ImportParam Parameters used when importing/loading assets
	 * \tparam ExportParam Parameters used when exporting/saving assets
	 */
	template <typename T, typename ImportParam, typename ExportParam>
	class AssetLoader
	{
	public:

		/**
		 * \brief Default destructor
		 */
		virtual ~AssetLoader() = default;	

		/**
		 * \brief Loads an asset from a file path matching importer capabilities
		 *
		 * Iterates through registered importers and returns the first valid asset
		 * that successfully supports the given path extension and import parameters.
		 *
		 * \param path The file path of the asset to load
		 * \param parameters Import parameters used to configure the import
		 *
		 * \return A shared pointer to the loaded asset, or nullptr if no importer succeeds
		 */
		std::shared_ptr<T> LoadFromPath(const Path& path, const ImportParam& parameters) const
		{
			String extension = path.GetExtension();
			for (AssetImporter<T, ImportParam>* importer : mImporters)
			{
				if (!importer->SupportExtension(extension))
					continue;

				if (!importer->SupportParameters(parameters))
					continue;

				std::shared_ptr<T> asset = importer->LoadFromPath(path, parameters);
				if (!asset)
					continue;

				return asset;
			}

			return nullptr;
		}

		/**
		 * \brief Saves an asset to a file path using the first matching exporter
		 *
		 * Searches for an exporter supporting both the file extension and export
		 * parameters. If found, the exporter handles the saving operation.
		 *
		 * \param asset The asset instance to save
		 * \param path The destination file path
		 * \param parameters Export parameters used to configure the export
		 */
		void SaveToPath(const T& asset, const Path& path, const ExportParam& parameters) const
		{
			String extension = path.GetExtension();
			for (AssetExporter<T, ExportParam>* exporter : mExporters)
			{
				if (!exporter->SupportExtension(extension))
					continue;

				if (!exporter->SupportParameters(parameters))
					continue;

				exporter->SaveToPath(asset, path, parameters);
				return;
			}
		}

		/**
		 * \brief Loads an asset from a stream using supported importers
		 *
		 * Tries each importer that supports the given import parameters. If an
		 * importer fails, the input stream is rewound before trying the next one.
		 *
		 * \param inputStream Input stream containing serialized asset data.
		 * \param parameters Import parameters used to configure the import
		 *
		 * \return A shared pointer to the loaded asset, or nullptr on failure.
		 */
		std::shared_ptr<T> LoadFromStream(InputStream<>& inputStream, const ImportParam& parameters = {}) const
		{
			for (AssetImporter<T, ImportParam>* importer : mImporters)
			{
				if (!importer->SupportParameters(parameters))
					continue;

				std::shared_ptr<T> asset = importer->LoadFromStream(inputStream, parameters);
				if (!asset)
				{
					inputStream.Rewind();
					continue;
				}

				return asset;
			}

			return nullptr;
		}

		/**
		 * \brief Saves an asset to an output stream using the first matching exporter
		 *
		 * \param asset The asset to serialize
		 * \param outputStream Output stream to write serialized data to
		 * \param parameters Export parameters used to configure the export
		 */
		void SaveToStream(const T& asset, OutputStream<>& outputStream, const ExportParam& parameters = {}) const
		{
			for (AssetExporter<T, ExportParam>* exporter : mExporters)
			{
				if (!exporter->SupportParameters(parameters))
					continue;

				exporter->SaveToStream(asset, outputStream, parameters);
				return;
			}
		}

		/**
		 * \brief Loads an asset from a raw memory buffer
		 *
		 * \param buffer Pointer to the memory block storing asset data
		 * \param length Size of the buffer in bytes
		 * \param parameters Import parameters used to configure the import
		 *
		 * \return A shared pointer to the loaded asset, or nullptr on failure
		 */
		std::shared_ptr<T> LoadFromMemory(const Byte* buffer, std::size_t length, const ImportParam& parameters = {}) const
		{
			for (AssetImporter<T, ImportParam>* importer : mImporters)
			{
				if (!importer->SupportParameters(parameters))
					continue;

				std::shared_ptr<T> asset = importer->LoadFromMemory(buffer, length, parameters);
				if (!asset)
					continue;

				return asset;
			}

			return nullptr;
		}

		/**
		 * \brief Saves an asset from a raw memory buffer
		 *
		 * \param asset The asset to serialize
		 * \param parameters Export parameters used to configure the export
		 *
	     * \return A byte array containing serialized asset data or empty on failure
		 */
		ByteArray SaveToMemory(const T& asset, const ExportParam& parameters = {}) const
		{
			for (AssetExporter<T, ExportParam>* exporter : mExporters)
			{
				if (!exporter->SupportParameters(parameters))
					continue;

				return exporter->SaveToMemory(asset, parameters);
			}

			return {};
		}

		/**
		 * \brief Checks whether both import and export operations support a file extension
		 *
		 * \param extension The file extension to test
		 * \return True if at least one importer and one exporter support the extension
		 */
		bool SupportExtension(StringView extension)
		{
			return SupportExtensionImport(extension) && SupportExtensionExport(extension);
		}

		/**
		 * \brief Checks whether any importer supports a given file extension
		 *
		 * \param extension The file extension to test
		 * \return True if supported by at least one importer
		 */
		bool SupportExtensionImport(StringView extension)
		{
			return mImporters.AnyOf([&](AssetImporter<T, ImportParam>* importer) {
				return importer->SupportExtension(extension);
			});
		}

		/**
		 * \brief Checks whether any exporter supports a given file extension
		 *
		 * \param extension The file extension to test
		 * \return True if supported by at least one exporter
		 */
		bool SupportExtensionExport(StringView extension)
		{
			return mExporters.AnyOf([&](AssetExporter<T, ExportParam>* exporter) {
				return exporter->SupportExtension(extension);
			});
		}

		/**
		 * \brief Registers a new importer to support additional asset formats
		 *
		 * \param importer The importer instance to register
		 */
		void RegisterImporter(AssetImporter<T, ImportParam>* importer)
		{
			mImporters.Add(importer);
		}


		/**
		 * \brief Registers a new exporter to support additional output formats
		 *
		 * \param exporter The exporter instance to register
		 */
		void RegisterExporter(AssetExporter<T, ExportParam>* exporter)
		{
			mExporters.Add(exporter);
		}

	private:

		Array<AssetImporter<T, ImportParam>*> mImporters;
		Array<AssetExporter<T, ExportParam>*> mExporters;
	};
}

#endif // COCKTAIL_CORE_ASSET_ASSETLOADER_HPP
