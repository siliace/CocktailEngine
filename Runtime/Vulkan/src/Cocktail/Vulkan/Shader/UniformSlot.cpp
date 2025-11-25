#include <Cocktail/Vulkan/Shader/UniformSlot.hpp>

namespace Ck::Vulkan
{
	class MyUniformMember : public Renderer::UniformMember
	{
	public:

		MyUniformMember(UniformSlot* slot, const BlockMember& blockMember) :
			mSlot(slot),
			mBlock(blockMember)
		{
			mMembers.Reserve(blockMember.Members.GetSize());
			for (unsigned int i = 0; i < mMembers.GetSize(); i++)
				mMembers.Emplace(new MyUniformMember(mSlot, mBlock.Members[i]));
		}

		const AsciiString& GetName() const override
		{
			return mBlock.Name;
		}

		const DataType& GetDataType() const override
		{
			return mBlock.DataType;
		}

		unsigned int GetElementCount() const override
		{
			return mBlock.ElementCount;
		}

		unsigned int GetArrayLength() const override
		{
			return mBlock.ArrayLength;
		}

		std::size_t GetMemberCount() const override
		{
			return mBlock.Members.GetSize();
		}

		std::size_t GetMembers(UniformMember** members, std::size_t memberCount, std::size_t firstMember) const override
		{
			std::size_t i = 0;
			const std::size_t total = mMembers.GetSize();
			if (firstMember < total)
			{
				for (; i < memberCount && i + firstMember < total; i++)
					members[i] = mMembers[i + firstMember].Get();
			}

			return i;
		}

		UniformSlot* GetSlot() const override
		{
			return mSlot;
		}

		std::size_t GetOffset() const override
		{
			return mBlock.Offset;
		}

	private:

		UniformSlot* mSlot;
		BlockMember mBlock;
		Array<UniquePtr<MyUniformMember>> mMembers;
	};

	UniformSlot::UniformSlot(Renderer::ShaderProgramType programType, const Array<BlockMember>& members, AsciiString name, const DescriptorSetLayoutBinding& layoutBindingInfo, unsigned int set) :
		mProgramType(programType),
		mName(std::move(name)),
		mLayoutBindingInfo(layoutBindingInfo),
		mSet(set)
	{
		mMembers.Reserve(members.GetSize());
		for (unsigned int i = 0; i < members.GetSize(); i++)
			mMembers.Emplace(new MyUniformMember(this, members[i]));
	}

	Renderer::ShaderProgramType UniformSlot::GetProgramType() const
	{
		return mProgramType;
	}

	Renderer::DescriptorType UniformSlot::GetDescriptorType() const
	{
		return mLayoutBindingInfo.Type;
	}

	bool UniformSlot::IsArray() const
	{
		return mLayoutBindingInfo.DescriptorCount > 1;
	}

	unsigned int UniformSlot::GetArrayLength() const
	{
		return mLayoutBindingInfo.DescriptorCount;
	}

	std::size_t UniformSlot::GetMemberCount() const
	{
		return mMembers.GetSize();
	}

	std::size_t UniformSlot::GetMembers(Renderer::UniformMember** members, std::size_t memberCount, std::size_t firstMember) const
	{
		std::size_t i = 0;
		const std::size_t total = mMembers.GetSize();
		if (firstMember < total)
		{
			for (; i < memberCount && i + firstMember < total; i++)
				members[i] = mMembers[i + firstMember].Get();
		}

		return i;
	}

	Flags<Renderer::ShaderType> UniformSlot::GetShaderStages() const
	{
		return mLayoutBindingInfo.ShaderStages;
	}

	const AsciiString& UniformSlot::GetName() const
	{
		return mName;
	}

	unsigned int UniformSlot::GetBinding() const
	{
		return mLayoutBindingInfo.Binding;
	}
	
	unsigned int UniformSlot::GetSet() const
	{
		return mSet;
	}
}
