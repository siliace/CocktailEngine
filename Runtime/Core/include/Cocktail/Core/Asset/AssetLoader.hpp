#ifndef COCKTAIL_CORE_ASSET_ASSETLOADER_HPP
#define COCKTAIL_CORE_ASSET_ASSETLOADER_HPP

#include <Cocktail/Core/Asset/AssetExporter.hpp>
#include <Cocktail/Core/Asset/AssetImporter.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T, typename ImportParam, typename ExportParam>
	class AssetLoader
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~AssetLoader() = default;	

		/**
		 * \brief 
		 * \param path 
		 * \param parameters 
		 * \return 
		 */
		Ref<T> LoadFromPath(const std::filesystem::path& path, const ImportParam& parameters) const
		{
			std::string extension = path.extension().string();
			for (AssetImporter<T, ImportParam>* importer : mImporters)
			{
				if (!importer->SupportExtension(extension))
					continue;

				if (!importer->SupportParameters(parameters))
					continue;

				Ref<T> asset = importer->LoadFromPath(path, parameters);
				if (!asset)
					continue;

				return asset;
			}

			return nullptr;
		}

		/**
		 * \brief 
		 * \param asset 
		 * \param path 
		 * \param parameters 
		 */
		void SaveToPath(const T& asset, const std::filesystem::path& path, const ExportParam& parameters) const
		{
			std::string extension = path.extension().string();
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
		 * \brief
		 * \param inputStream
		 * \param parameters
		 * \return
		 */
		Ref<T> LoadFromStream(InputStream& inputStream, const ImportParam& parameters = {}) const
		{
			for (AssetImporter<T, ImportParam>* importer : mImporters)
			{
				if (!importer->SupportParameters(parameters))
					continue;

				Ref<T> asset = importer->LoadFromStream(inputStream, parameters);
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
		 * \brief
		 * \param asset
		 * \param outputStream
		 * \param parameters
		 */
		void SaveToStream(const T& asset, OutputStream& outputStream, const ExportParam& parameters = {}) const
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
		 * \brief
		 * \param buffer
		 * \param length
		 * \param parameters
		 * \return
		 */
		Ref<T> LoadFromMemory(const void* buffer, std::size_t length, const ImportParam& parameters = {}) const
		{
			for (AssetImporter<T, ImportParam>* importer : mImporters)
			{
				if (!importer->SupportParameters(parameters))
					continue;

				Ref<T> asset = importer->LoadFromMemory(buffer, length, parameters);
				if (!asset)
					continue;

				return asset;
			}

			return nullptr;
		}

		/**
		 * \brief
		 * \param asset
		 * \param buffer
		 * \param length
		 * \param parameters
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
		 * \brief 
		 * \param extension 
		 * \return 
		 */
		bool SupportExtension(std::string_view extension)
		{
			return SupportExtensionImport(extension) && SupportExtensionExport(extension);
		}

		/**
		 * \brief 
		 * \param extension 
		 * \return 
		 */
		bool SupportExtensionImport(std::string_view extension)
		{
			for (AssetImporter<T, ImportParam>* importer : mImporters)
			{
				if (importer->SupportExtension(extension))
					return true;
			}

			return false;
		}

		/**
		 * \brief 
		 * \param extension 
		 * \return 
		 */
		bool SupportExtensionExport(std::string_view extension)
		{
			for (AssetExporter<T, ExportParam>* exporter : mExporters)
			{
				if (exporter->SupportExtension(extension))
					return true;
			}

			return false;
		}

		/**
		 * \brief
		 * \param importer
		 */
		void RegisterImporter(AssetImporter<T, ImportParam>* importer)
		{
			mImporters.push_back(importer);
		}

		/**
		 * \brief 
		 * \param exporter 
		 */
		void RegisterExporter(AssetExporter<T, ExportParam>* exporter)
		{
			mExporters.push_back(exporter);
		}

	private:

		std::vector<AssetImporter<T, ImportParam>*> mImporters;
		std::vector<AssetExporter<T, ExportParam>*> mExporters;
	};
}

#endif // COCKTAIL_CORE_ASSET_ASSETLOADER_HPP
