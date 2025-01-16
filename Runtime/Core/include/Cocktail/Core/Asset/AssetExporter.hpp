#ifndef COCKTAIL_CORE_ASSET_ASSETEXPORTER_HPP
#define COCKTAIL_CORE_ASSET_ASSETEXPORTER_HPP

#include <filesystem>

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 * \tparam P 
	 */
	template <typename T, typename P>
	class AssetExporter
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~AssetExporter() = default;	

		/**
		 * \brief
		 * \param asset
		 * \param path
		 * \param parameters
		 */
		virtual void SaveToPath(const T& asset, const std::filesystem::path& path, const P& parameters = {}) const = 0;

		/**
		 * \brief
		 * \param asset
		 * \param outputStream
		 * \param parameters
		 */
		virtual void SaveToStream(const T& asset, OutputStream& outputStream, const P& parameters = {}) const = 0;

		/**
		 * \brief
		 * \param asset
		 * \param parameters
		 * \return 
		 */
		virtual ByteArray SaveToMemory(const T& asset, const P& parameters = {}) const = 0;

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

#endif // COCKTAIL_CORE_ASSET_ASSETEXPORTER_HPP
