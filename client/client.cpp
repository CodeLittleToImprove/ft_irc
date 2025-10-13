// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h> // need for signal cmd
#include <cerrno>
#include <cstdio>

int main()
{
	signal(SIGPIPE, SIG_IGN);  // Ignore SIGPIPE globally
	// creating socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	// specifying address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// sending connection request
	if (connect(clientSocket, (struct sockaddr*)&serverAddress, // connect is counterpart to accept
			sizeof(serverAddress)) < 0)
	{
		perror("connect failed");
		close(clientSocket);
		return 1;
	}
	std::cout << "Connected to server! Type messages to send.\n";

	// sending data
	// const char* message = "Hello, server!";
	// const char* message = ;

	std::string message;
	while (true)
	{
		std::cout << "> ";
		std::getline(std::cin, message); // \n gets striped from string

		message += "\r\n";
		std::cout << "[debug] sending message bytes: "; // d stands for \r and a for \n
		for (size_t i = 0; i < message.size(); ++i)
			std::cout << std::hex << (int)(unsigned char)message[i] << ' ';
		std::cout << std::dec << std::endl;


		ssize_t sent = send(clientSocket, message.c_str(), message.size(), 0);
		if (sent == -1)
		{
			if (errno == EPIPE)
				std::cout << "Server closed connection (EPIPE).\n";
			else
				perror("Error in sending message");
			break;
		}
		if (message == "quit\r\n")
			break;
	}
	// closing socket
	std::cout << "Closing client connection.\n";
	close(clientSocket);

	return 0;
}
