#ifndef COCKTAIL_CORE_ASSET_ASSETMANAGER_HPP
#define COCKTAIL_CORE_ASSET_ASSETMANAGER_HPP

#include <unordered_map>

#include <Cocktail/Core/Asset/AssetImporter.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class AssetManager : public Implements<AssetManager<T>, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param name 
		 * \return 
		 */
		Ref<T> Get(const std::string& name) const
		{
			
		}

		/**
		 * \brief 
		 * \param asset 
		 * \param name 
		 */
		void Set(const Ref<T>& asset, const std::string& name)
		{
			
		}

	private:
		
		std::unordered_map<std::string, Ref<T>> mAssets;
	};
}

#endif // COCKTAIL_CORE_ASSET_ASSETMANAGER_HPP
