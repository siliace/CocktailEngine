#include <tinyxml2.h>

#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlCDataNode.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

namespace Ck
{
	namespace
	{
		std::pair<String, XmlProperties::ElementType> ProcessXmlElement(const tinyxml2::XMLElement& xmlElement)
		{
			XmlProperties::ElementType element;
			for (const tinyxml2::XMLAttribute* xmlAttribute = xmlElement.FirstAttribute(); xmlAttribute; xmlAttribute = xmlAttribute->Next())
			{
				const bool isProtectedAttribute = StringUtils<AnsiChar>::StartsWith(xmlAttribute->Name(), "__");
				if (isProtectedAttribute)
					continue;

				element.Insert(CK_ANSI_TO_TEXT(xmlAttribute->Name()), XmlProperties::ValueType(xmlAttribute->Value()));
			}

			for (const tinyxml2::XMLNode* xmlChildNode = xmlElement.FirstChild(); xmlChildNode; xmlChildNode = xmlChildNode->NextSibling())
			{
				if (const tinyxml2::XMLElement* xmlChildElement = xmlChildNode->ToElement())
				{
					auto [childName, childElement] = ProcessXmlElement(*xmlChildElement);
					if (childName.IsEmpty())
						throw XmlParseException(CK_TEXT("Element with empty name"));

					if (element.HasChild(childName))
					{
						XmlProperties::NodeType& child = element.GetChild(childName);
						if (child.GetType() == XmlProperties::NodeType::Type::Sequence)
						{
							XmlProperties::SequenceType* sequence = static_cast<XmlProperties::SequenceType*>(&child);
							sequence->Add(childElement);
						}
						else
						{
							XmlProperties::SequenceType sequence;
							sequence.Add(child);
							sequence.Add(childElement);

							element.Insert(childName, sequence);
						}
					}
					else
					{
						if (xmlChildElement->BoolAttribute("__is_sequence"))
						{
							XmlProperties::SequenceType sequence;
							sequence.Add(childElement);

							element.Insert(childName, sequence);
						}
						else
						{
							element.Insert(childName, childElement);
						}
					}
				}
				else if (const tinyxml2::XMLText* xmlChildText = xmlChildNode->ToText())
				{
					if (xmlChildText->CData())
					{
						const char* data = xmlChildText->Value();

						String childName = CK_TEXT("CData");
						if (const AnsiChar* cdataName = xmlChildText->Parent()->ToElement()->Attribute("__cdata_name"))
							childName = CK_ANSI_TO_TEXT(cdataName);

						std::size_t length = xmlChildText->Parent()->ToElement()->Unsigned64Attribute("__cdata_length");
						if (!length)
							length = StringUtils<AnsiChar>::GetLength(data);

						element.Insert(childName, XmlCDataNode(length, data));
					}
				}
			}

			return { CK_ANSI_TO_TEXT(xmlElement.Value()), std::move(element) };
		}
	}

	XmlProperties::XmlProperties(const Path& path)
	{
		tinyxml2::XMLDocument document;
		String xml = FileUtils::ReadFile(path).ToString();

		tinyxml2::XMLError error = document.Parse(CK_TEXT_TO_ANSI(xml.GetData()), xml.GetLength());
		if (error != tinyxml2::XML_SUCCESS)
			throw std::runtime_error(document.ErrorStr());

		tinyxml2::XMLElement* xmlRoot = document.RootElement();
		auto [name, element] = ProcessXmlElement(*xmlRoot);

		ResetRoot(name, element);
	}
}
