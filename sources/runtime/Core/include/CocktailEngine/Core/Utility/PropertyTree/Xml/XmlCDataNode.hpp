#ifndef COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP
#define COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP

#include <CocktailEngine/Core/Utility/ByteArray.hpp>
#include <CocktailEngine/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

namespace Ck
{
	class XmlCDataNode : public XmlProperties::CustomNodeType
	{
	public:

		XmlCDataNode(std::size_t length, const Byte* data);

		const ByteArray& Data() const;

	protected:

		XmlCDataNode(XmlProperties::NodeType* parent, const String& name, ByteArray data);

		UniquePtr<XmlProperties::NodeType> Clone(XmlProperties::NodeType* parent, const String& name) const override;

	private:

		ByteArray mData;
	};
}

#endif // COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_XML_XMLCDATANODE_HPP
