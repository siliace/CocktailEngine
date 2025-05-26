#ifndef COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP
#define COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP

#include <filesystem>

#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T, typename P>
	class AssetImporter
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~AssetImporter() = default;	

		/**
		 * \brief 
		 * \param path
		 * \param parameters 
		 * \return 
		 */
		virtual std::shared_ptr<T> LoadFromPath(const std::filesystem::path& path, const P& parameters = {})
		{
			FileInputStream inputStream(Storage::OpenFile(path, FileOpenFlagBits::Read));
			return LoadFromStream(inputStream, parameters);
		}

		/**
		 * \brief 
		 * \param inputStream 
		 * \param parameters
		 * \return 
		 */
		virtual std::shared_ptr<T> LoadFromStream(InputStream& inputStream, const P& parameters = {}) = 0;

		/**
		 * \brief 
		 * \param buffer 
		 * \param length 
		 * \param parameters
		 * \return 
		 */
		virtual std::shared_ptr<T> LoadFromMemory(const void* buffer, std::size_t length, const P& parameters = {})
		{
			MemoryInputStream inputStream(buffer, length);
			return LoadFromStream(inputStream, parameters);
		}

		/**
		 * \brief 
		 * \param extension 
		 * \return 
		 */
		virtual bool SupportExtension(std::string_view extension) const = 0;

		/**
		 * \brief
		 * \param parameters
		 * \return
		 */
		virtual bool SupportParameters(const P& parameters) const = 0;
	};
}

#endif // COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP
