#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP

#include <filesystem>

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTree.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API XmlProperties : public Properties
	{
	public:
		/**
		 * \brief 
		 * \param path 
		 */
		explicit XmlProperties(const std::filesystem::path& path);
	};

	class XmlCDataNode : public XmlProperties::CustomNode
	{
	public:

		XmlCDataNode(std::size_t length, const void* data)
		{
			mData.reserve(length);
			for (std::size_t i = 0; i < length; i++)
				mData.push_back(*(static_cast<const Uint8*>(data) + i));
		}

		const std::vector<Uint8>& Data() const
		{
			return mData;
		}

	protected:

		XmlCDataNode(XmlProperties::Node* parent, const std::string& name, std::vector<Uint8> data) :
			XmlProperties::CustomNode(parent, std::move(name)),
			mData(std::move(data))
		{
			/// Nothing
		}

		std::unique_ptr<XmlProperties::Node> Clone(XmlProperties::Node* parent, const std::string& name) const override
		{
			return std::unique_ptr<XmlProperties::Node>(
				new XmlCDataNode(parent, name, mData)
			);
		}

	private:
		std::vector<Uint8> mData;
	};
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
