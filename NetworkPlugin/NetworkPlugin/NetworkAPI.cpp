#include "NetworkAPI.h"

NETWORK_API UnityPlugin::NetworkAPI* CreateNetwork()
{
	return new UnityPlugin::NetworkAPI();
}

NETWORK_API void DeleteNetwork(UnityPlugin::NetworkAPI* networkPtr)
{
	delete networkPtr;
	networkPtr = NULL;
}

NETWORK_API unsigned long long CreateServerSocket(UnityPlugin::NetworkAPI* networkPtr)
{
	if (networkPtr == NULL)
		return 1;

	const unsigned long long socketServer = networkPtr->CreateServerSocket();

	return socketServer;
}

NETWORK_API unsigned long long CreateClientSocket(UnityPlugin::NetworkAPI* networkPtr)
{
	if (networkPtr == NULL)
		return 1;

	const unsigned long long socketClient = networkPtr->CreateClientSocket();

	return socketClient;
}

NETWORK_API SOCKADDR_IN* CreateServerAddress(UnityPlugin::NetworkAPI* networkPtr, unsigned short port)
{
	if (networkPtr == NULL)
		return NULL;

	SOCKADDR_IN* address =  new SOCKADDR_IN(networkPtr->CreateServerAddress(port));

	return address;
}

NETWORK_API SOCKADDR_IN* CreateClientAddress(UnityPlugin::NetworkAPI* networkPtr, const char* ip, unsigned short port)
{
	if (networkPtr == NULL)
		return NULL;

	SOCKADDR_IN* address = new SOCKADDR_IN(networkPtr->CreateClientAddress(ip, port));

	return address;
}

NETWORK_API void DeleteSockAddress(SOCKADDR_IN* address)
{
	if (address == NULL)
		return;

	delete address;
	address = nullptr;
}

NETWORK_API void BindServerAddress(UnityPlugin::NetworkAPI* networkPtr, unsigned long long sock, SOCKADDR_IN address)
{
	if (networkPtr == NULL)
		return;

	networkPtr->BindServerAddress(sock, address);
}

NETWORK_API void RegisterDebugCallback(DebugCallback callback)
{
	if (callback)
	{
		UnityPlugin::debugCallback = callback;
	}
}

NETWORK_API void CloseSocket(UnityPlugin::NetworkAPI* networkPtr, unsigned long long socket)
{
	if (networkPtr == NULL)
		return;

	networkPtr->CloseSocket(socket);
}

NETWORK_API int ReceiveFrom(UnityPlugin::NetworkAPI* networkPtr, SOCKET sock, unsigned char* dataPacket, SOCKADDR_IN sockClient)
{
	if (networkPtr == NULL)
		return 1;

	return networkPtr->RecvFrom(sock, dataPacket, sockClient);
}

UnityPlugin::NetworkAPI::NetworkAPI()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	DebugInUnity("WSAS Init");
}

UnityPlugin::NetworkAPI::~NetworkAPI()
{
	WSACleanup();
}

SOCKET UnityPlugin::NetworkAPI::CreateServerSocket()
{
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	DebugInUnity("Server Socket Created");

	return sock;
}

SOCKET UnityPlugin::NetworkAPI::CreateClientSocket()
{
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	DWORD nonBlocking = 1;
	ioctlsocket(sock, FIONBIO, &nonBlocking);

	DebugInUnity("Client Socket Created");

	return sock;
}

SOCKADDR_IN UnityPlugin::NetworkAPI::CreateServerAddress(unsigned short p_port)
{
	SOCKADDR_IN address;
	address.sin_family = AF_INET;
	address.sin_port = htons(p_port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	DebugInUnity("Server Address Created");

	return address;
}

SOCKADDR_IN UnityPlugin::NetworkAPI::CreateClientAddress(const char* p_ip, unsigned short p_port)
{
	SOCKADDR_IN address;
	address.sin_family = AF_INET;
	address.sin_port = htons(p_port);

	inet_pton(AF_INET, p_ip, &address.sin_addr);

	DebugInUnity("Client Address Created");

	return address;
}

void UnityPlugin::NetworkAPI::BindServerAddress(SOCKET p_sock, SOCKADDR_IN p_address)
{
	if (bind(p_sock, reinterpret_cast<const SOCKADDR*>(&p_address), sizeof(p_address)) == SOCKET_ERROR)
		DebugInUnity("Failed to Bind Server");
	else
		DebugInUnity("Server Binded");
}

int UnityPlugin::NetworkAPI::RecvFrom(SOCKET p_sock, unsigned char* p_dataPacket, SOCKADDR_IN p_sockClient)
{
	char* data = reinterpret_cast<char*>(p_dataPacket);

	int clientSize = sizeof(p_sockClient);
	const int bytesReceive = recvfrom(p_sock, data, sizeof(data), 0, reinterpret_cast<SOCKADDR*>(&p_sockClient), &clientSize);

	if (bytesReceive <= 0)
		DebugInUnity("Failed to Receive packets");
		
	return bytesReceive;
}

bool UnityPlugin::NetworkAPI::SendTo(SOCKET& p_sock, const unsigned char* p_dataPacket, SOCKADDR_IN& p_sockClient)
{
	const char* data = reinterpret_cast<const char*>(p_dataPacket);

	const int clientSize = sizeof(p_sockClient);
	const int bytesSend = sendto(p_sock, data, sizeof(data), 0, reinterpret_cast<const SOCKADDR*>(&p_sockClient), clientSize);

	if (bytesSend != sizeof(p_dataPacket))
	{
		DebugInUnity("Failed to Send packets");
		return false;
	}
	
	return true;
}

void UnityPlugin::NetworkAPI::DebugInUnity(const std::string& p_message)
{
	if(debugCallback)
	{
		debugCallback(p_message.c_str());
	}
}

void UnityPlugin::NetworkAPI::CloseSocket(SOCKET p_sock)
{
	closesocket(p_sock);
	DebugInUnity("Socket Closed");
}