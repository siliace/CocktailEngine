#ifndef COCKTAIL_CORE_ASSET_ASSETLOADER_HPP
#define COCKTAIL_CORE_ASSET_ASSETLOADER_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Asset/AssetExporter.hpp>
#include <Cocktail/Core/Asset/AssetImporter.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T, typename ImportParam, typename ExportParam>
	class AssetLoader : public Extends<AssetLoader<T, ImportParam, ExportParam>, Object>
	{
	public:

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
			for (const Ref<AssetImporter<T, ImportParam>>& importer : mImporters)
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
			for (const Ref<AssetExporter<T, ExportParam>>& exporter : mExporters)
			{
				if (exporter->SupportExtension(extension))
					return true;
			}

			return false;
		}

		/**
		 * \brief 
		 * \param path 
		 * \param parameters 
		 * \return 
		 */
		Ref<T> LoadFromPath(const std::filesystem::path& path, const ImportParam& parameters) const
		{
			std::string extension = path.extension().string();
			for (const Ref<AssetImporter<T, ImportParam>>& importer : mImporters)
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
		void SaveToPath(const Ref<T>& asset, const std::filesystem::path& path, const ExportParam& parameters) const
		{
			std::string extension = path.extension().string();
			for (const Ref<AssetExporter<T, ExportParam>>& exporter : mExporters)
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
		Ref<T> LoadFromStream(const Ref<InputStream>& inputStream, const ImportParam& parameters = {}) const
		{
			for (const Ref<AssetImporter<T, ImportParam>>& importer : mImporters)
			{
				if (!importer->SupportParameters(parameters))
					continue;

				Ref<T> asset = importer->LoadFromStream(inputStream, parameters);
				if (!asset)
				{
					inputStream->Rewind();
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
		void SaveToStream(const Ref<T>& asset, const Ref<OutputStream>& outputStream, const ExportParam& parameters = {}) const
		{
			for (const Ref<AssetExporter<T, ExportParam>>& exporter : mExporters)
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
			for (const Ref<AssetImporter<T, ImportParam>>& importer : mImporters)
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
		void SaveToMemory(const Ref<T>& asset, void* buffer, std::size_t length, const ExportParam& parameters = {}) const
		{
			for (const Ref<AssetExporter<T, ExportParam>>& exporter : mExporters)
			{
				if (!exporter->SupportParameters(parameters))
					continue;

				exporter->SaveToMemory(asset, buffer, length, parameters);
				return;
			}
		}

		/**
		 * \brief 
		 * \param exporter 
		 */
		void RegisterExporter(const Ref<AssetExporter<T, ExportParam>>& exporter)
		{
			mExporters.push_back(exporter);
		}

		/**
		 * \brief 
		 * \param importer 
		 */
		void RegisterImporter(const Ref<AssetImporter<T, ImportParam>>& importer)
		{
			mImporters.push_back(importer);
		}

	private:

		std::vector<Ref<AssetExporter<T, ExportParam>>> mExporters;
		std::vector<Ref<AssetImporter<T, ImportParam>>> mImporters;
	};
}

#endif // COCKTAIL_CORE_ASSET_ASSETLOADER_HPP
