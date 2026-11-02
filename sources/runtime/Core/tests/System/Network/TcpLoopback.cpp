#include <cstring>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketPoller.hpp>
#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/System/Network/TcpListener.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace
{
    const Ck::Duration LoopbackTimeout = Ck::Duration::Seconds(2);
    const Ck::Duration AcceptRetryInterval = Ck::Duration::Milliseconds(1);

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

TEST_CASE("TCP loopback exposes endpoints and transfers data", "[Network][Loopback][Tcp]")
{
    Ck::TcpListener listener;
    Ck::TcpClient client;
    Ck::Optional<Ck::TcpClient> acceptedClient;

    REQUIRE(listener.GetLocalAddress().IsEmpty());
    REQUIRE(listener.GetLocalPort().IsEmpty());
    REQUIRE(client.GetLocalAddress().IsEmpty());
    REQUIRE(client.GetLocalPort().IsEmpty());

    listener.Bind(Ck::IpAddressV4::Localhost, 0);
    listener.Listen();

    Ck::Optional<const Ck::IpAddress&> listenerAddress = listener.GetLocalAddress();
    REQUIRE_FALSE(listenerAddress.IsEmpty());
    RequireLoopbackAddress(listenerAddress.Get());

    Ck::Optional<unsigned short> listenerPort = listener.GetLocalPort();
    REQUIRE_FALSE(listenerPort.IsEmpty());
    REQUIRE(listenerPort.Get() != 0);

    {
        Ck::SocketPoller poller;
        poller.Add(listener, ReadEvents());

        client.Connect(Ck::IpAddressV4::Localhost, listenerPort.Get());

        REQUIRE(poller.Wait(LoopbackTimeout));
        REQUIRE(poller.IsReadyToRead(listener));
        poller.Remove(listener);

        acceptedClient = listener.TryAccept(LoopbackTimeout, AcceptRetryInterval);
        REQUIRE_FALSE(acceptedClient.IsEmpty());

        Ck::Optional<const Ck::IpAddress&> clientLocalAddress = client.GetLocalAddress();
        REQUIRE_FALSE(clientLocalAddress.IsEmpty());
        RequireLoopbackAddress(clientLocalAddress.Get());

        Ck::Optional<unsigned short> clientLocalPort = client.GetLocalPort();
        REQUIRE_FALSE(clientLocalPort.IsEmpty());
        REQUIRE(clientLocalPort.Get() != 0);

        Ck::Optional<const Ck::IpAddress&> clientRemoteAddress = client.GetRemoteAddress();
        REQUIRE_FALSE(clientRemoteAddress.IsEmpty());
        RequireLoopbackAddress(clientRemoteAddress.Get());
        REQUIRE(client.GetRemotePort().Get() == listenerPort.Get());

        Ck::Optional<const Ck::IpAddress&> acceptedLocalAddress = acceptedClient.Get().GetLocalAddress();
        REQUIRE_FALSE(acceptedLocalAddress.IsEmpty());
        RequireLoopbackAddress(acceptedLocalAddress.Get());
        REQUIRE(acceptedClient.Get().GetLocalPort().Get() == listenerPort.Get());
        REQUIRE(acceptedClient.Get().GetRemotePort().Get() == clientLocalPort.Get());

        const char request[] = "ping";
        char receivedRequest[sizeof(request)] = {};

        poller.Add(acceptedClient.Get(), ReadEvents());
        client.Send(request, static_cast<unsigned int>(sizeof(request)));

        REQUIRE(poller.Wait(LoopbackTimeout));
        REQUIRE(poller.IsReadyToRead(acceptedClient.Get()));
        poller.Remove(acceptedClient.Get());

        REQUIRE(acceptedClient.Get().Receive(receivedRequest, static_cast<unsigned int>(sizeof(receivedRequest))) == sizeof(receivedRequest));
        REQUIRE(std::memcmp(receivedRequest, request, sizeof(request)) == 0);

        const char response[] = "pong";
        char receivedResponse[sizeof(response)] = {};

        poller.Add(client, ReadEvents());
        acceptedClient.Get().Send(response, static_cast<unsigned int>(sizeof(response)));

        REQUIRE(poller.Wait(LoopbackTimeout));
        REQUIRE(poller.IsReadyToRead(client));
        poller.Remove(client);

        REQUIRE(client.Receive(receivedResponse, static_cast<unsigned int>(sizeof(receivedResponse))) == sizeof(receivedResponse));
        REQUIRE(std::memcmp(receivedResponse, response, sizeof(response)) == 0);
    }
}
