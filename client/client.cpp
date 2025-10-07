// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
	// creating socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	// specifying address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// sending connection request
	connect(clientSocket, (struct sockaddr*)&serverAddress, // connect is counterpart to accept
			sizeof(serverAddress));

	// sending data
	// const char* message = "Hello, server!";
	// const char* message = ;
	std::cout << "Connected to server! Type messages to send.\n";
	std::cout << "(Type '*' to start sending message to server)\n";
	std::cout << "(Type '#' to quit)\n";
	std::string message;
	while (true)
	{
		std::cout << "> ";
		std::getline(std::cin, message);
		send(clientSocket, message.c_str(), message.size(), 0);
		if (message.find('#') != std::string::npos)
			break;
	}
	// closing socket
	close(clientSocket);

	return 0;
}