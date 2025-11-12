#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>

namespace Ck
{
    const IpAddressV4 IpAddressV4::Any(0, 0, 0, 0);
    const IpAddressV4 IpAddressV4::Localhost(127, 0, 0, 1);
    const IpAddressV4 IpAddressV4::Broadcast(255, 255, 255, 255);

    IpAddressV4::IpAddressV4(Uint32 ipAddress)
    {
        mBytes = Unpack(ipAddress);
    }

    IpAddressV4::IpAddressV4(Uint8 first, Uint8 second, Uint8 third, Uint8 forth) :
        mBytes()
    {
        mBytes[0] = first;
        mBytes[1] = second;
        mBytes[2] = third;
        mBytes[3] = forth;
    }

    std::size_t IpAddressV4::GetLength() const
    {
	    return mBytes.size();
    }

    Uint8 IpAddressV4::GetByte(std::size_t index) const
    {
	    assert(index < mBytes.size());
	    return mBytes[index];
    }

    void IpAddressV4::SetByte(std::size_t index, Uint8 byte)
    {
	    assert(index < mBytes.size());
	    mBytes[index] = byte;
    }

    Uint32 IpAddressV4::ToInteger() const
    {
        return Pack(mBytes);
    }

    IpAddressV4::Protocol IpAddressV4::GetProtocol() const
    {
        return Protocol::Ipv4;
    }

    String IpAddressV4::ToString() const
    {
        String result;
        for (String::SizeType i = 0; i < mBytes.size(); i++)
        {
            if (i > 0)
                result.Append(CK_CHAR('.'));

            result.Append(TranslatorCast<String>(mBytes[i]));
        }

        return result;
    }

    bool IpAddressV4::operator==(const IpAddressV4& rhs) const
    {
	    return mBytes == rhs.mBytes;
    }

    bool IpAddressV4::operator!=(const IpAddressV4& rhs) const
    {
	    return !(*this == rhs);
    }
}
