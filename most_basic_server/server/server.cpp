// C++ program to show the example of server application in
// socket programming
#include <cstring> // for memset
#include <iostream>
#include <netinet/in.h> // needed for sockaddr_in
#include <sys/socket.h> // for portability
#include <unistd.h> // for close
#include <limits>

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
	// int min = std::numeric_limits<float>::min();
	// std::cout << min << std::endl;
	int portnbr = 8080;
	// creating socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // input: IPv4 protocol, TCP socket, default

	// specifying the address
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress)); // just nice to do not necessary
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portnbr); // converts to network byte order
	serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP

	// binding socket.
	bind(serverSocket, (struct sockaddr*)&serverAddress,
	     sizeof(serverAddress));

	// listening to the assigned socket
	listen(serverSocket, 5);

	// accepting connection request
	int clientSocket
		= accept(serverSocket, NULL, NULL);

	// receiving data
	char buffer[1024] = {0};
	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Message from client: " << buffer
		<< std::endl;

	// closing the socket.
	close(serverSocket);

	return 0;
}
