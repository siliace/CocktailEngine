#include <tinyxml2.h>

#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

#include "Cocktail/Core/Utility/StringUtils.hpp"

namespace Ck
{
	namespace
	{
		std::pair<std::string, XmlProperties::Element> ProcessXmlElement(const tinyxml2::XMLElement& xmlElement)
		{
			XmlProperties::Element element;
			for (const tinyxml2::XMLAttribute* xmlAttribute = xmlElement.FirstAttribute(); xmlAttribute; xmlAttribute = xmlAttribute->Next())
				element.Insert(xmlAttribute->Name(), XmlProperties::Value(xmlAttribute->Value()));

			for (const tinyxml2::XMLNode* xmlChildNode = xmlElement.FirstChild(); xmlChildNode; xmlChildNode = xmlChildNode->NextSibling())
			{
				if (const tinyxml2::XMLElement* xmlChildElement = xmlChildNode->ToElement())
				{
					auto [childName, childElement] = ProcessXmlElement(*xmlChildElement);
					if (childName.empty())
						throw std::runtime_error("Parse xml element with empty name");

					if (element.HasChild(childName))
					{
						XmlProperties::Sequence* sequence;
						XmlProperties::Node& child = element.GetChild(childName);
						if (child.GetType() == XmlProperties::Node::Type::Sequence)
						{
							sequence = static_cast<XmlProperties::Sequence*>(&child);
						}
						else
						{
							sequence = element.Insert(childName, XmlProperties::Sequence());
						}
						sequence->Add(childElement);
					}
					else
					{
						element.Insert(childName, childElement);
					}
				}
			}

			return { xmlElement.Value(), std::move(element) };
		}
	}

	XmlProperties::XmlProperties(const std::filesystem::path& path)
	{
		tinyxml2::XMLDocument document;
		std::string xml = FileUtils::ReadFile(path);

		tinyxml2::XMLError error = document.Parse(xml.c_str(), xml.length());
		if (error != tinyxml2::XML_SUCCESS)
			throw std::runtime_error(document.ErrorStr());

		tinyxml2::XMLElement* xmlRoot = document.RootElement();
		auto [name, element] = ProcessXmlElement(*xmlRoot);

		ResetRoot(name, std::move(element));
	}
}
