#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlCDataNode.hpp>

namespace Ck
{
	XmlCDataNode::XmlCDataNode(std::size_t length, const void* data) :
		mData(data, length)
	{
		/// Nothing
	}

	const ByteArray& XmlCDataNode::Data() const
	{
		return mData;
	}

	XmlCDataNode::XmlCDataNode(XmlProperties::Node* parent, const std::string& name, ByteArray data) :
		XmlProperties::CustomNode(parent, name),
		mData(std::move(data))
	{
		/// Nothing
	}

	std::unique_ptr<XmlProperties::Node> XmlCDataNode::Clone(XmlProperties::Node* parent, const std::string& name) const
	{
		return std::unique_ptr<XmlProperties::Node>(
			new XmlCDataNode(parent, name, mData)
		);
	}
}
