#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP

#include <Cocktail/Core/Utility/ByteArray.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

namespace Ck
{
	class XmlCDataNode : public XmlProperties::CustomNodeType
	{
	public:

		XmlCDataNode(std::size_t length, const void* data);

		const ByteArray& Data() const;

	protected:

		XmlCDataNode(XmlProperties::NodeType* parent, const String& name, ByteArray data);

		std::unique_ptr<XmlProperties::NodeType> Clone(XmlProperties::NodeType* parent, const String& name) const override;

	private:

		ByteArray mData;
	};
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP
