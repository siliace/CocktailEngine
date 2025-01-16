#include <tinyxml2.h>

#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

namespace Ck
{
	namespace
	{
		std::pair<std::string, XmlProperties::Element> ProcessXmlElement(const tinyxml2::XMLElement& xmlElement)
		{
			XmlProperties::Element element;
			for (const tinyxml2::XMLAttribute* xmlAttribute = xmlElement.FirstAttribute(); xmlAttribute; xmlAttribute = xmlAttribute->Next())
			{
				if (StringUtils::StartsWith(xmlAttribute->Name(), "__"))
					continue;

				element.Insert(xmlAttribute->Name(), XmlProperties::Value(xmlAttribute->Value()));
			}

			for (const tinyxml2::XMLNode* xmlChildNode = xmlElement.FirstChild(); xmlChildNode; xmlChildNode = xmlChildNode->NextSibling())
			{
				if (const tinyxml2::XMLElement* xmlChildElement = xmlChildNode->ToElement())
				{
					auto [childName, childElement] = ProcessXmlElement(*xmlChildElement);
					if (childName.empty())
						throw std::runtime_error("Parse xml element with empty name");

					if (element.HasChild(childName))
					{
						XmlProperties::Node& child = element.GetChild(childName);
						if (child.GetType() == XmlProperties::Node::Type::Sequence)
						{
							XmlProperties::Sequence* sequence = static_cast<XmlProperties::Sequence*>(&child);
							sequence->Add(childElement);
						}
						else
						{
							XmlProperties::Sequence sequence;
							sequence.Add(child);
							sequence.Add(childElement);

							element.Insert(childName, sequence);
						}
					}
					else
					{
						if (xmlChildElement->BoolAttribute("__is_sequence"))
						{
							XmlProperties::Sequence sequence;
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

						std::string childName = "CData";
						if (const char* cdataName = xmlChildText->Parent()->ToElement()->Attribute("__cdata_name"))
							childName.assign(cdataName);

						std::size_t length = xmlChildText->Parent()->ToElement()->Unsigned64Attribute("__cdata_length");
						if (!length)
							length = std::strlen(data);

						element.Insert(childName, XmlCDataNode(length, data));
					}
				}
			}

			return { xmlElement.Value(), std::move(element) };
		}
	}

	XmlProperties::XmlProperties(const std::filesystem::path& path)
	{
		tinyxml2::XMLDocument document;
		std::string xml = FileUtils::ReadFile(path).ToString();

		tinyxml2::XMLError error = document.Parse(xml.c_str(), xml.length());
		if (error != tinyxml2::XML_SUCCESS)
			throw std::runtime_error(document.ErrorStr());

		tinyxml2::XMLElement* xmlRoot = document.RootElement();
		auto [name, element] = ProcessXmlElement(*xmlRoot);

		ResetRoot(name, element);
	}
}
