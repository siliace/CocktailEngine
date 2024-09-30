#ifndef COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP
#define COCKTAIL_CORE_ASSET_ASSETIMPORTER_HPP

#include <filesystem>

#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T, typename P>
	class AssetImporter : public Implements<AssetImporter<T, P>, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param path
		 * \param parameters 
		 * \return 
		 */
		virtual Ref<T> LoadFromPath(const std::filesystem::path& path, const P& parameters = {}) const = 0;

		/**
		 * \brief 
		 * \param inputStream 
		 * \param parameters
		 * \return 
		 */
		virtual Ref<T> LoadFromStream(const Ref<InputStream>& inputStream, const P& parameters = {}) const = 0;

		/**
		 * \brief 
		 * \param buffer 
		 * \param length 
		 * \param parameters
		 * \return 
		 */
		virtual Ref<T> LoadFromMemory(const void* buffer, std::size_t length, const P& parameters = {}) const = 0;

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
