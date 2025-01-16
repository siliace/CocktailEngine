#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP

#include <filesystem>

#include <Cocktail/Core/Utility/ByteArray.hpp>
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

		XmlCDataNode(std::size_t length, const void* data) :
			mData(data, length)
		{
			/// Nothing
		}

		const ByteArray& Data() const
		{
			return mData;
		}

	protected:

		XmlCDataNode(XmlProperties::Node* parent, const std::string& name, ByteArray data) :
			XmlProperties::CustomNode(parent, name),
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

		ByteArray mData;
	};
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
