#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP

#include <Cocktail/Core/Utility/ByteArray.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

namespace Ck
{
	class XmlCDataNode : public XmlProperties::CustomNode
	{
	public:

		XmlCDataNode(std::size_t length, const void* data);

		const ByteArray& Data() const;

	protected:

		XmlCDataNode(XmlProperties::Node* parent, const std::string& name, ByteArray data);

		std::unique_ptr<XmlProperties::Node> Clone(XmlProperties::Node* parent, const std::string& name) const override;

	private:

		ByteArray mData;
	};
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP
