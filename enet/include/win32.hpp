#ifndef __ENET_WIN32_H__
#define __ENET_WIN32_H__

#include <stdlib.h>
#include <winsock2.h>

typedef SOCKET ENetSocket;

#define ENET_SOCKET_NULL INVALID_SOCKET

#define ENET_HOST_TO_NET_16(value) (htons (value))
#define ENET_HOST_TO_NET_32(value) (htonl (value))

#define ENET_NET_TO_HOST_16(value) (ntohs (value))
#define ENET_NET_TO_HOST_32(value) (ntohl (value))

typedef struct
{
	size_t dataLength;
	void* data;
} ENetBuffer;

#define ENET_CALLBACK __cdecl
#define ENET_API extern  

typedef fd_set ENetSocketSet;

#define ENET_SOCKETSET_EMPTY(sockset)          FD_ZERO (& (sockset))
#define ENET_SOCKETSET_ADD(sockset, socket)    FD_SET (socket, & (sockset))
#define ENET_SOCKETSET_REMOVE(sockset, socket) FD_CLR (socket, & (sockset))
#define ENET_SOCKETSET_CHECK(sockset, socket)  FD_ISSET (socket, & (sockset))

#endif   
