// C++ program to show the example of server application in
// socket programming
#include <cstring> // for memset
#include <iostream>
#include <netinet/in.h> // needed for sockaddr_in
#include <sys/socket.h> // for portability
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
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // input: IPv4 protocol, TCP socket, default

	// specifying the address
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress)); // just nice to do not necessary
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portnbr); // converts to network byte order
	serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP

	// binding socket to address
	bind(serverSocket, (struct sockaddr*)&serverAddress,
	     sizeof(serverAddress));

	// listening to the assigned socket
	listen(serverSocket, 5); // n = how many pending connections can be queued
	std::cout << "Server listening on port " << portnbr << "..." << std::endl;

	sockaddr_in clientAddress;
	socklen_t clientAddressLen = sizeof(clientAddress);
	// accepting client connection
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
	// accepts ands you a new socket FD representing one side of an already-established TCP connection.

	if (clientSocket < 0)
	{
		perror("accept failed");
		exit(EXIT_FAILURE);
	}

	bool sessionActive = true;
	char buffer[1024];
	while (sessionActive) // means here is an accepted connection
	{
		memset(buffer, 0, sizeof(buffer));
		// receiving data from client
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived <= 0)
		{
			std::cout << "Client disconnected" << std::endl;
			break;
		}
		for (int i = 0; i < bytesReceived; i++)
		{
			char c = buffer[i];
			if (c == '*')
			{
				std::cout << "Client can start to send message" << std::endl;
				std::cout << "bytesReceived: " << bytesReceived << std::endl;
				break;
			}

			else if (c == '#')
			{
				std::cout << "Client wants to end the connection" << std::endl;
				sessionActive = false;
				break;
			}
			// else
			// 	std::cout << c;
		}
		std::cout << "bytesReceived: " << bytesReceived << std::endl;
		std::cout << buffer << std::endl;
	}
	close(clientSocket);
	close(serverSocket);
	return 0;
}