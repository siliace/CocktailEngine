#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

#include <Cocktail/Graphic/Material/Shading/VertexInterface.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class NamedVertexInterface : public Inherit<NamedVertexInterface, Object, VertexInterface>
	{
	public:

		/**
		 * \brief 
		 * \param prefix 
		 * \param suffix 
		 * \param caseSensitive 
		 */
		NamedVertexInterface(std::string prefix, std::string suffix, bool caseSensitive = true) :
			mPrefix(std::move(prefix)),
			mSuffix(std::move(suffix)),
			mCaseSensitive(caseSensitive)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param shaderProgram 
		 * \param semantic 
		 * \return 
		 */
		Renderer::VertexAttributeLocation* GetAttributeLocation(const Renderer::ShaderProgram& shaderProgram, VertexAttributeSemantic semantic) const override
		{
			Ref<Renderer::Shader> vertexStage = shaderProgram.GetStage(Renderer::ShaderType::Vertex);
			if (!vertexStage)
				return nullptr;

			std::string expectedName = mPrefix;
			expectedName = expectedName.append(Enum<VertexAttributeSemantic>::ToString(semantic));
			if (!mSuffix.empty())
				expectedName = expectedName.append(mSuffix);

			Renderer::VertexAttributeLocation* vertexAttributeLocation;
			for (unsigned int i = 0; i < vertexStage->GetInputAttributeCount(); i++)
			{
				vertexStage->GetInputAttributes(&vertexAttributeLocation, 1, i);
				if (StringUtils::Compare(vertexAttributeLocation->GetName(), expectedName, mCaseSensitive))
					return vertexAttributeLocation;
			}

			return nullptr;
		}

	private:

		std::string mPrefix;
		std::string mSuffix;
		bool mCaseSensitive;
	};

	Ref<VertexInterface> VertexInterface::FromNames(const std::string& prefix, const std::string& suffix, bool caseSensitive)
	{
		return NamedVertexInterface::New(prefix, suffix, caseSensitive);
	}
}
