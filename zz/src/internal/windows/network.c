#ifdef ZZ_WINDOWS

#include "zz/internal/network.h"
#include "network.h"

#include "zz/log.h"
#include "zz/assert.h"

static struct zz_internal_network internal_network;

struct zz_network_ip_endpoint zz_network_ip_endpoint_fill(u8 a, u8 b, u8 c, u8 d, u32 port)
{
    return (struct zz_network_ip_endpoint){(a << 24) | (b << 16) | (c << 8) | (d << 0), port};
}

b8 zz_network_ip_endpoint_equals(const struct zz_network_ip_endpoint* a, const struct zz_network_ip_endpoint* b)
{
    return a->address == b->address && a->port == b->port;
}

SOCKADDR_IN sockaddr_in_from_ip_endpoint(const struct zz_network_ip_endpoint* ip_endpoint)
{
    SOCKADDR_IN sockaddr_in = {};
    sockaddr_in.sin_family = AF_INET;
    sockaddr_in.sin_addr.s_addr = htonl(ip_endpoint->address);
    sockaddr_in.sin_port = htons(ip_endpoint->port);
    return sockaddr_in;
}

static SOCKET (*windows_socket)(int af, int type, int protocol) = socket;
b8 socket_create(struct socket* socket, const struct socket_config* config)
{
    socket->ip_endpoint = config->ip_endpoint;

    socket->socket = windows_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int size = 1024 * 1024;
    int actual_size;
    int length = sizeof(int);

    if (setsockopt(socket->socket, SOL_SOCKET, SO_SNDBUF, (char*)&size, length) == SOCKET_ERROR)
    {
        return ZZ_FALSE;
    }
    if (getsockopt(socket->socket, SOL_SOCKET, SO_SNDBUF, (char*)&actual_size, &length) == SOCKET_ERROR)
    {
        return ZZ_FALSE;
    }
    ZZ_ASSERT(actual_size == size);

    if (setsockopt(socket->socket, SOL_SOCKET, SO_RCVBUF, (char*)&size, length) == SOCKET_ERROR)
    {
        return ZZ_FALSE;
    }
    if (getsockopt(socket->socket, SOL_SOCKET, SO_RCVBUF, (char*)&actual_size, &length) == SOCKET_ERROR)
    {
        return ZZ_FALSE;
    }
    ZZ_ASSERT(actual_size == size);

    u_long enabled = 1;
    if (ioctlsocket(socket->socket, FIONBIO, &enabled) == SOCKET_ERROR)
    {
        return ZZ_FALSE;
    }

    SOCKADDR_IN local_address = sockaddr_in_from_ip_endpoint(&socket->ip_endpoint);
    if (bind(socket->socket, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
    {
        return ZZ_FALSE;
    }

    return ZZ_TRUE;
}

void socket_destroy(struct socket* socket)
{
    ZZ_ASSERT(closesocket(socket->socket) != SOCKET_ERROR);
}

b8 socket_send(struct socket* socket, struct zz_network_packet* packet)
{
    SOCKADDR_IN to_address;
	to_address.sin_family = AF_INET;
	to_address.sin_addr.S_un.S_addr = htonl(packet->ip_endpoint.address);
	to_address.sin_port = htons(packet->ip_endpoint.port);
	if (sendto(socket->socket, (const char*)packet->buffer, packet->size, 0, (SOCKADDR*)&to_address, sizeof(to_address)) == SOCKET_ERROR)
	{
		return ZZ_FALSE;
	}

	return ZZ_TRUE;
}

b8 socket_receive(struct socket* socket, struct zz_network_packet* packet)
{
	SOCKADDR_IN from_address;
    int from_address_size = sizeof(from_address);
	int size = recvfrom(socket->socket, (char*)packet->buffer, 1024, 0, (SOCKADDR*)&from_address, &from_address_size);
	if (size == SOCKET_ERROR)
	{
		return ZZ_FALSE;
	}

	packet->size = size;
    packet->ip_endpoint.address = (u32)(ntohl(from_address.sin_addr.S_un.S_addr));
    packet->ip_endpoint.port = (u32)(ntohs(from_address.sin_port));

	return ZZ_TRUE;
}

b8 zz_internal_network_initialize(struct zz_internal_network_config* config)
{

    WSADATA winsock_data;
    if (WSAStartup(0x202, &winsock_data))
    {
        return ZZ_FALSE;
    }

    struct socket_config socket_config;
    socket_config.ip_endpoint = config->ip_endpoint;
    if (!socket_create(&internal_network.socket, &socket_config))
    {
        ZZ_LOG_FATAL("Failed to create socket: %d.", WSAGetLastError());
        return FALSE;
    }

    return ZZ_TRUE;
}

void zz_internal_network_deinitialize()
{
    socket_destroy(&internal_network.socket);
}

b8 zz_internal_network_send(struct zz_network_packet* packet)
{
    return socket_send(&internal_network.socket, packet);
}

b8 zz_internal_network_receive(struct zz_network_packet* packet)
{
    return socket_receive(&internal_network.socket, packet);
}

#endif