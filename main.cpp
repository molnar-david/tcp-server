#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

int main()
{
	// Initialize WinSock
	WSADATA wsData{};
	WORD ver{ MAKEWORD(2, 2) };

	int ret{ WSAStartup(ver, &wsData) };
	if (ret != 0)
	{
		std::cerr << "Can't initialize WinSock! Error: " << ret << '\n';
		return 0;
	}

	// Create a socket
	SOCKET listening{ socket(AF_INET, SOCK_STREAM, 0) };
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket! Error: " << WSAGetLastError() << '\n';
		WSACleanup();
		return 0;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint{};
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);	// host to network short
	hint.sin_addr.S_un.S_addr = INADDR_ANY;	// Could also use inet_pton ...

	ret = bind(listening, (sockaddr*)&hint, sizeof(hint));
	if (ret == SOCKET_ERROR)
	{
		std::cerr << "Can't bind socket! Error: " << WSAGetLastError() << '\n';
		closesocket(listening);
		WSACleanup();
		return 0;
	}

	// Tell WinSock the socket is for listening
	ret = listen(listening, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		std::cerr << "Can't listen on socket! Error: " << WSAGetLastError() << '\n';
		closesocket(listening);
		WSACleanup();
		return 0;
	}

	// Wait for a connection
	sockaddr_in client{};
	int clientSize{ sizeof(client) };

	SOCKET clientSocket{ accept(listening, (sockaddr*)&client, &clientSize) };
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Can't accept a client! Error: " << WSAGetLastError() << '\n';
		closesocket(listening);
		WSACleanup();
		return 0;
	}

	char host[NI_MAXHOST]{};		// Client's remote name
	char service[NI_MAXSERV]{};		// Service (i. e. port) the client is connected on

	ret = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (ret != 0)
	{
		std::cerr << "Can't get client name info! Error: " << WSAGetLastError() << '\n';
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
	}

	std::cout << host << " connected on port " << ntohs(client.sin_port) << '\n';

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];

	while (true)
	{
		// Wait for client to send data
		int bytesReceived{ recv(clientSocket, buf, 4096, 0) };
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Can't read from " << host << "! Error: " << WSAGetLastError() << '\n';
			break;
		}

		// Client disconnects
		if (bytesReceived == 0)
		{
			std::cout << host << " disconnected\n";
			break;
		}

		// Echo message back to client
		ret = send(clientSocket, buf, bytesReceived, 0);
		if (ret == SOCKET_ERROR)
		{
			std::cerr << "Can't send data to " << host << "! Error: " << WSAGetLastError() << '\n';
			break;
		}
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup WinSock
	WSACleanup();

	return 0;
}