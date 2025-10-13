
#include <cstring> // for memset
#include <iostream>
#include <netinet/in.h> // needed for sockaddr_in
#include <sys/socket.h> // for portability
#include <sys/types.h>
#include <arpa/inet.h> // for inet_pton
#include <unistd.h> // for close
#include <cstdio> // for perror
#include <stdlib.h> // for exit
#include <stdexcept> // for std:run_time_error
#include <cerrno>

// 1. creating server socket
int createServerSocket()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // input: IPv4 protocol, TCP socket, default , also returns a fd
	if (serverSocket == -1)
		throw std::runtime_error("Socket creation failed");
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 	// reuse port when server gets started to fast
	{
		close(serverSocket);
		throw std::runtime_error("setsockopt failed: " + std::string(strerror(errno)));
	}
	return serverSocket;
}

// 2. creating serverAddress
sockaddr_in createServerAddress(uint16_t port)
{
	// this is how a sockaddrr_in looks like
	// struct sockaddr_in
	// {
	// 	sa_family_t sin_family; // Address family (always AF_INET for IPv4)
	// 	in_port_t sin_port; // Port number (must be in network byte order)
	// 	struct in_addr sin_addr; // IPv4 address (struct with a 32-bit value)
	// 	unsigned char sin_zero[8]; // Padding (not used)
	// };
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress)); // just nice to do not necessary
	// specifying the address
	serverAddress.sin_family = AF_INET; // format of ipaddress
	serverAddress.sin_port = htons(port); // converts to network byte order
	serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP
	// inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr); // works similar to INADDR_ANY
	return serverAddress;
}

// 3. bind the socket to a port
void bindServerSocket(int serverSocket, const sockaddr_in &serverAddress)
{
	int bindResult = bind(serverSocket, (struct sockaddr *) &serverAddress,
		 sizeof(serverAddress));
	if (bindResult == -1)
		throw std::runtime_error(std::string("Bind failed: ") + std::strerror(errno));
}

// 4. listen for incoming connections
void listenServerSocket(int serverSocket, int backlog)
{
	// listening to the assigned socket
	int listenResult = listen(serverSocket, backlog); // n = how many pending connections can be queued
	if (listenResult == -1)
		throw std::runtime_error(std::string("Listen failed: ") + std::strerror(errno));
}

// 5. accept a client connection
int acceptClient(int serverSocket, sockaddr_in &clientAddress)
{

	socklen_t clientAddressLen = sizeof(clientAddress);
	// accepting client connection
	// accepts and adds you a new socket FD representing one side of an already-established TCP connection.
	int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLen);
	if (clientSocket == -1)
		throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
	std::cout << "Client connected." << std::endl;
	return clientSocket;
}

int main()
{
	uint16_t port = 8080; // typical uint16_t is used for tdp and udp ports
	size_t queue_size = 5;
	int serverSocket = createServerSocket();
	sockaddr_in serverAddress = createServerAddress(port);
	bindServerSocket(serverSocket, serverAddress);
	listenServerSocket(serverSocket, queue_size);
	std::cout << "Server listening on port " << port << "..." << std::endl;
	sockaddr_in clientAddress;
	int clientSocket = acceptClient(serverSocket, clientAddress);

	std::string buffer;
	char recvBuf[512];
	while (true)
	{
		int bytesReceived = recv(clientSocket, recvBuf, sizeof(recvBuf) - 1, 0);
		if (bytesReceived <= 0)
		{
			std::cout << "bytes received: "<< bytesReceived << std::endl;
			std::cout << "Client disconnected" << std::endl;
			break;
		}

		recvBuf[bytesReceived] = '\0';
		buffer += recvBuf;

		size_t end;

		// print buffer before filtering
		// std::cout << "unfiltered buffer (escaped): ";
		// for (size_t i = 0; i < buffer.size(); ++i)
		// {
		// 	unsigned char c = buffer[i];
		// 	if (isprint(c))
		// 		std::cout << c;
		// 	else
		// 		std::cout << "\\x" << std::hex << (int)c << std::dec;
		// }
		// std::cout << std::endl;

		while ((end = buffer.find("\r\n")) != std::string::npos)
		{
			std::string message = buffer.substr(0, end);
			buffer.erase(0, end + 2);
			std::cout << "Cleaned message: " << message << std::endl;
			// handleIRCCommand(message);  // parse and respond
		}
	}
	close(clientSocket);
	close(serverSocket);
	std::cout << "closing server connection" << std::endl;
	return 0;
}
