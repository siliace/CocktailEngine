#ifndef COCKTAIL_RENDERER_SHADER_UNIFORMSLOT_HPP
#define COCKTAIL_RENDERER_SHADER_UNIFORMSLOT_HPP

#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/Flags.hpp>
#include <Cocktail/Core/String.hpp>

#include <Cocktail/Renderer/Shader/DescriptorType.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramType.hpp>
#include <Cocktail/Renderer/Shader/ShaderType.hpp>

namespace Ck::Renderer
{
	class UniformSlot;

	/**
	 * \brief 
	 */
	class UniformMember
	{
	public:

		/**
		 * \brief 
		 */
		virtual ~UniformMember() = default;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const AnsiString& GetName() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const DataType& GetDataType() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual unsigned int GetElementCount() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual unsigned int GetArrayLength() const = 0;

		/**
		 * \brief 
		 * \return The number of member
		 */
		virtual std::size_t GetMemberCount() const = 0;

		/**
		 * \brief
		 * \param members
		 * \param memberCount
		 * \param firstMember
		 * \return
		 */
		virtual std::size_t GetMembers(UniformMember** members, std::size_t memberCount, std::size_t firstMember = 0) const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual UniformSlot* GetSlot() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::size_t GetOffset() const = 0;
	};

	/**
	 * \brief Interface describing the location of an uniform value from a ShaderProgram
	 */
	class UniformSlot
	{
	public:

		/**
		 * \brief
		 */
		virtual ~UniformSlot() = default;

		/**
		 * \brief Get the program type this uniform location is compatible with
		 * \return The program type
		 */
		virtual ShaderProgramType GetProgramType() const = 0;

		/**
		 * \brief Get the descriptor type of the value
		 * \return The descriptor type
		 */
		virtual DescriptorType GetDescriptorType() const = 0;

		/**
		 * \brief Tell whether the uniform is an array of descriptor
		 * Array slots allow to bind various resources on the same slot
		 * \return True if the descriptor is an array, false otherwise
		 */
		virtual bool IsArray() const = 0;

		/**
         * \brief 
         * \return
         */
        virtual unsigned int GetArrayLength() const = 0;

		/**
		 * \brief Get the number of data members at the root of the uniform
		 * \return The number of member
		 */
		virtual std::size_t GetMemberCount() const = 0;

		/**
		 * \brief 
		 * \param members 
		 * \param memberCount 
		 * \param firstMember 
		 * \return 
		 */
		virtual std::size_t GetMembers(UniformMember** members, std::size_t memberCount, std::size_t firstMember = 0) const = 0;

		/**
		 * \brief Get a flag mask describing where the uniform will be available
		 * \return Flags of stages
		 */
		virtual Flags<ShaderType> GetShaderStages() const = 0;

		/**
		 * \brief Get the name of the descriptor in the shader code
		 * \return The name of the uniform if found in the stage
		 */
		virtual const AnsiString& GetName() const = 0;
	};
}

#endif // COCKTAIL_RENDERER_SHADER_UNIFORMSLOT_HPP
