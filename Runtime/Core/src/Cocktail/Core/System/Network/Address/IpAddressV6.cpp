#include <Cocktail/Core/System/Network/Address/IpAddressV6.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

namespace Ck
{
	const IpAddressV6 IpAddressV6::Any(0, 0, 0, 0, 0, 0, 0, 0);

	const IpAddressV6 IpAddressV6::Localhost(0, 0, 0, 0, 0, 0, 0, 1);

	IpAddressV6::IpAddressV6(Uint16 a, Uint16 b, Uint16 c, Uint16 d, Uint16 e, Uint16 f, Uint16 g, Uint16 h)
	{
		Shorts[0] = a >> 8 | a << 8;
		Shorts[1] = b >> 8 | b << 8;
		Shorts[2] = c >> 8 | c << 8;
		Shorts[3] = d >> 8 | d << 8;
		Shorts[4] = e >> 8 | e << 8;
		Shorts[5] = f >> 8 | f << 8;
		Shorts[6] = g >> 8 | g << 8;
		Shorts[7] = h >> 8 | h << 8;
	}

	std::size_t IpAddressV6::GetLength() const
	{
		return Bytes.size();
	}

	Uint8 IpAddressV6::GetByte(std::size_t index) const
	{
		assert(index < Bytes.size());
		return Bytes[index];
	}

	void IpAddressV6::SetByte(std::size_t index, Uint8 value)
	{
		assert(index < Bytes.size());
		Bytes[index] = value;
	}

	Uint16 IpAddressV6::GetShort(std::size_t index) const
	{
		assert(index < Shorts.size());
		return Shorts[index];
	}

	void IpAddressV6::SetShort(std::size_t index, Uint16 value)
	{
		assert(index < Shorts.size());
		Shorts[index] = value;
	}

	IpAddress::Protocol IpAddressV6::GetProtocol() const
	{
		return Protocol::Ipv6;
	}

	std::string IpAddressV6::ToString() const
	{
		return StringUtils::Join(Bytes.begin(), Bytes.end(), ":");
	}
}
