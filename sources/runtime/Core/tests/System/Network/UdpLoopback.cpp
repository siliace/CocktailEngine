#include <cstring>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketAddress.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketPoller.hpp>
#include <CocktailEngine/Core/System/Network/UdpSocket.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace
{
    const Ck::Duration LoopbackTimeout = Ck::Duration::Seconds(2);

    void RequireLoopbackAddress(const Ck::IpAddress& address)
    {
        REQUIRE(address.GetProtocol() == Ck::IpAddress::Protocol::Ipv4);
        REQUIRE(address.ToString() == Ck::IpAddressV4::Localhost.ToString());
    }

    Ck::SocketPoller::EventFlags ReadEvents()
    {
        return Ck::SocketPoller::EventFlags::Of(Ck::SocketPoller::EventFlagBits::Read);
    }
}

TEST_CASE("UDP loopback transfers one datagram between ephemeral endpoints", "[Network][Loopback][Udp]")
{
    Ck::UdpSocket sender;
    Ck::UdpSocket receiver;

    REQUIRE(sender.GetLocalAddress().IsEmpty());
    REQUIRE(sender.GetLocalPort().IsEmpty());
    REQUIRE(receiver.GetLocalAddress().IsEmpty());
    REQUIRE(receiver.GetLocalPort().IsEmpty());

    sender.Bind(Ck::IpAddressV4::Localhost, 0);
    receiver.Bind(Ck::IpAddressV4::Localhost, 0);

    Ck::Optional<const Ck::IpAddress&> senderAddress = sender.GetLocalAddress();
    REQUIRE_FALSE(senderAddress.IsEmpty());
    RequireLoopbackAddress(senderAddress.Get());

    Ck::Optional<unsigned short> senderPort = sender.GetLocalPort();
    REQUIRE_FALSE(senderPort.IsEmpty());
    REQUIRE(senderPort.Get() != 0);

    Ck::Optional<const Ck::IpAddress&> receiverAddress = receiver.GetLocalAddress();
    REQUIRE_FALSE(receiverAddress.IsEmpty());
    RequireLoopbackAddress(receiverAddress.Get());

    Ck::Optional<unsigned short> receiverPort = receiver.GetLocalPort();
    REQUIRE_FALSE(receiverPort.IsEmpty());
    REQUIRE(receiverPort.Get() != 0);

    {
        Ck::SocketPoller poller;
        poller.Add(receiver, ReadEvents());

        const char datagram[] = "ping";
        sender.Send(datagram, sizeof(datagram), Ck::IpAddressV4::Localhost, receiverPort.Get());

        REQUIRE(poller.Wait(LoopbackTimeout));
        REQUIRE(poller.IsReadyToRead(receiver));
        poller.Remove(receiver);

        char receivedDatagram[sizeof(datagram)] = {};
        Ck::SocketAddress senderEndpoint;
        REQUIRE(receiver.Receive(receivedDatagram, sizeof(receivedDatagram), senderEndpoint) == sizeof(receivedDatagram));
        REQUIRE(std::memcmp(receivedDatagram, datagram, sizeof(datagram)) == 0);
        REQUIRE(senderEndpoint.GetProtocol() == Ck::IpAddress::Protocol::Ipv4);
        REQUIRE(senderEndpoint.GetAddressV4().ToString() == Ck::IpAddressV4::Localhost.ToString());
        REQUIRE(senderEndpoint.GetPort() == senderPort.Get());
    }
}