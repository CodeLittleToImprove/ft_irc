// C++ program to show the example of server application in
// socket programming
#include <cstring> // for memset
#include <iostream>
#include <netinet/in.h> // needed for sockaddr_in
#include <sys/socket.h> // for portability
#include <sys/types.h>
#include <arpa/inet.h> // for inet_pton
#include <unistd.h> // for close
#include <cstdio> // for perror
#include <limits>
#include <stdlib.h>

// this is how a sockaddrr_in looks like
// struct sockaddr_in
// {
// 	sa_family_t sin_family; // Address family (always AF_INET for IPv4)
// 	in_port_t sin_port; // Port number (must be in network byte order)
// 	struct in_addr sin_addr; // IPv4 address (struct with a 32-bit value)
// 	unsigned char sin_zero[8]; // Padding (not used)
// };

int main()
{
	int portnbr = 8080;
	// creating server socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // input: IPv4 protocol, TCP socket, default , also returns a fd
	if (serverSocket == -1)
	{
		std::cerr << "Socket creation failed." << std::endl;
		return -1;
	}

	// specifying the address
	sockaddr_in serverAddress; // sockaddr_in is ip address vs4?
	memset(&serverAddress, 0, sizeof(serverAddress)); // just nice to do not necessary
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portnbr); // converts to network byte order
	serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP
	// inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr); // works similar to INADDR_ANY

	// reuse port when server gets started to fast
	int opt = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// binding socket to address
	bind(serverSocket, (struct sockaddr *) &serverAddress,
	     sizeof(serverAddress));
	// todo add error handling to bind
	// listening to the assigned socket
	listen(serverSocket, 5); // n = how many pending connections can be queued
	std::cout << "Server listening on port " << portnbr << "..." << std::endl;

	sockaddr_in clientAddress;
	socklen_t clientAddressLen = sizeof(clientAddress);
	// accepting client connection
	int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLen);
	// accepts ands you a new socket FD representing one side of an already-established TCP connection.
	// todo add error handling to accept
	if (clientSocket < 0)
	{
		perror("accept failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Client connected." << std::endl;
	std::string buffer;
	char recvBuf[512];
	//
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
