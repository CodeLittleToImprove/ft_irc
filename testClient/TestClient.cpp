// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h> // need for signal cmd
#include <cerrno>
#include <cstdio> // for perror

#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	// Ignore SIGPIPE to avoid crash if server disconnects
	signal(SIGPIPE, SIG_IGN);

	const char* serverIP = "127.0.0.1";
	uint16_t serverPort = 8080;

	// 1. Create socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		perror("socket creation failed");
		return 1;
	}

	// 2. Specify server address
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr)); // zero out the struct
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	if (inet_aton(serverIP, &serverAddr.sin_addr) == 0)
	{
		std::cerr << "Invalid IP address\n";
		close(clientSocket);
		return 1;
	}

	// 3. Connect to server
	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("connect failed");
		close(clientSocket);
		return 1;
	}

	std::cout << "Connected to server at " << serverIP << ":" << serverPort << "\n";

	// 4. Simple loop: read from stdin, send to server
	char buffer[1024];
	while (true)
	{
		std::string message;
		std::cout << "> ";
		std::getline(std::cin, message);
		if (message.empty())
			continue;

		message += "\r\n"; // append line ending for server

		// Send to server
		ssize_t sent = send(clientSocket, message.c_str(), message.size(), 0);
		if (sent < 0)
		{
			perror("send failed");
			break;
		}

		if (message == "quit\r\n")
			break;

		// Try to receive any immediate server response (blocking)
		ssize_t received = recv(clientSocket, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
		if (received > 0)
		{
			buffer[received] = '\0';
			std::cout << "[server] " << buffer;
		}
	}

	// 5. Close socket
	std::cout << "Closing client connection.\n";
	close(clientSocket);

	return 0;
}
