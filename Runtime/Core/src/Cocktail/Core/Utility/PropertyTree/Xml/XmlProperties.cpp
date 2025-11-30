#include <tinyxml2.h>

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/Encoding/Encoders.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlCDataNode.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

#include "Cocktail/Core/IO/Input/Reader/BufferedReader.hpp"
#include "Cocktail/Core/IO/Input/Reader/FileReader.hpp"

namespace Ck
{
	namespace
	{
		std::pair<String, XmlProperties::ElementType> ProcessXmlElement(const tinyxml2::XMLElement& xmlElement)
		{
			XmlProperties::ElementType element;
			for (const tinyxml2::XMLAttribute* xmlAttribute = xmlElement.FirstAttribute(); xmlAttribute; xmlAttribute = xmlAttribute->Next())
			{
				AsciiStringView nameAttribute = xmlAttribute->Name();
				const bool isProtectedAttribute = StringUtils<AnsiChar, unsigned int>::StartsWith(nameAttribute.GetData(), "__");
				if (isProtectedAttribute)
					continue;

				String xmlAttributeValue = String::Convert(AsciiStringView(xmlAttribute->Value()));
				element.Insert(String::Convert(nameAttribute), XmlProperties::ValueType(xmlAttributeValue));
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
						if (AsciiStringView cdataName = xmlChildText->Parent()->ToElement()->Attribute("__cdata_name"); !cdataName.IsEmpty())
							childName = String::Convert(cdataName);

						std::size_t length = xmlChildText->Parent()->ToElement()->Unsigned64Attribute("__cdata_length");
						if (!length)
							length = StringUtils<AnsiChar, unsigned int>::GetLength(data);

						element.Insert(childName, XmlCDataNode(length, data));
					}
				}
			}

			AsciiStringView elementName = xmlElement.Value();
			return { String::Convert(elementName), std::move(element) };
		}
	}

	XmlProperties::XmlProperties(const Path& path)
	{
		tinyxml2::XMLDocument document;
		ByteArray content = FileUtils::ReadFile(path);
		Utf8String xml = Encoders::GetString<Encoders::Utf8, Utf8String>(content);

		tinyxml2::XMLError error = document.Parse(reinterpret_cast<const AnsiChar*>(xml.GetData()), xml.GetLength());
		if (error != tinyxml2::XML_SUCCESS)
			throw std::runtime_error(document.ErrorStr());

		tinyxml2::XMLElement* xmlRoot = document.RootElement();
		auto [name, element] = ProcessXmlElement(*xmlRoot);

		ResetRoot(name, element);
	}
}
