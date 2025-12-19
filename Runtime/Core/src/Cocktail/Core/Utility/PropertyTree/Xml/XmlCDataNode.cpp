#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlCDataNode.hpp>

namespace Ck
{
	XmlCDataNode::XmlCDataNode(std::size_t length, const Byte* data) :
		mData(data, length)
	{
		/// Nothing
	}

	const ByteArray& XmlCDataNode::Data() const
	{
		return mData;
	}

	XmlCDataNode::XmlCDataNode(XmlProperties::NodeType* parent, const String& name, ByteArray data) :
		XmlProperties::CustomNodeType(parent, name),
		mData(std::move(data))
	{
		/// Nothing
	}

	UniquePtr<XmlProperties::NodeType> XmlCDataNode::Clone(XmlProperties::NodeType* parent, const String& name) const
	{
		return UniquePtr<XmlProperties::NodeType>(
			new XmlCDataNode(parent, name, mData)
		);
	}
}
