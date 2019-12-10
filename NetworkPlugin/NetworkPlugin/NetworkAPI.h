#pragma once

#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

#define NETWORK_API __declspec(dllexport)

typedef void(__stdcall* DebugCallback)(const char* p_message);

namespace UnityPlugin {
	class NetworkAPI;
}

extern "C" {
	NETWORK_API	UnityPlugin::NetworkAPI*	CreateNetwork();
	NETWORK_API	void						DeleteNetwork(UnityPlugin::NetworkAPI* networkPtr);
	NETWORK_API	unsigned long long			CreateServerSocket(UnityPlugin::NetworkAPI* networkPtr);
	NETWORK_API	unsigned long long			CreateClientSocket(UnityPlugin::NetworkAPI* networkPtr);
	NETWORK_API SOCKADDR_IN*				CreateServerAddress(UnityPlugin::NetworkAPI* networkPtr, unsigned short port);
	NETWORK_API SOCKADDR_IN*				CreateClientAddress(UnityPlugin::NetworkAPI* networkPtr, const char* ip, unsigned short port);
	NETWORK_API	void						DeleteSockAddress(SOCKADDR_IN* address);
	NETWORK_API void						BindServerAddress(UnityPlugin::NetworkAPI* networkPtr, unsigned long long sock, SOCKADDR_IN address);
	NETWORK_API	void						RegisterDebugCallback(DebugCallback callback);
	NETWORK_API	void						CloseSocket(UnityPlugin::NetworkAPI* networkPtr, unsigned long long socket);
	NETWORK_API int							ReceiveFrom(UnityPlugin::NetworkAPI* networkPtr, SOCKET sock, unsigned char* dataPacket, SOCKADDR_IN sockClient);
}

//TODO SEND AND RECEIVE DATA -> STRING NOT UNSIGNED CHAR ARRAY

namespace UnityPlugin
{
	DebugCallback debugCallback;

	class NetworkAPI
	{
	public:
		NetworkAPI();
		~NetworkAPI();

		SOCKET			CreateServerSocket();
		SOCKET			CreateClientSocket();

		SOCKADDR_IN		CreateServerAddress(unsigned short p_port);
		SOCKADDR_IN		CreateClientAddress(const char* p_ip, unsigned short p_port);

		void			BindServerAddress(SOCKET p_sock, SOCKADDR_IN p_address);

		int				RecvFrom(SOCKET p_sock, unsigned char* p_dataPacket, SOCKADDR_IN p_sockClient);
		bool			SendTo(SOCKET& p_sock, const unsigned char* p_dataPacket, SOCKADDR_IN& p_sockClient);

		void			DebugInUnity(const std::string& p_message);
		void			CloseSocket(SOCKET p_sock);
	};
}