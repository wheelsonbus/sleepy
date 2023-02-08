#ifdef ZZ_WINDOWS
#ifndef ZZ_INTERNAL_WINDOWS_NETWORK_H
#define ZZ_INTERNAL_WINDOWS_NETWORK_H

#include <WinSock2.h>

#include "zz/memory.h"
#include "zz/network.h"

SOCKADDR_IN sockaddr_in_from_ip_endpoint(const struct zz_network_ip_endpoint* ip_endpoint);

struct socket
{
    SOCKET socket;
    struct zz_network_ip_endpoint ip_endpoint;
};

struct socket_config
{
    struct zz_network_ip_endpoint ip_endpoint;
};

b8 socket_create(struct socket* socket, const struct socket_config* config);
void socket_destroy(struct socket* socket);

b8 socket_send(struct socket* socket, struct zz_network_packet* packet);
b8 socket_receive(struct socket* socket, struct zz_network_packet* packet);

struct zz_internal_network
{
    struct socket socket;
};

#endif
#endif