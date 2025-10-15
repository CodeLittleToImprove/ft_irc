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
#include <cerrno> // for errno
#include <fcntl.h>
#include <vector>
#include <poll.h> // for pollfds
#include <iomanip> // for setw and setfill

void printEscapedBuffer(const std::string &buffer)
{
	std::cout << "unfiltered buffer (escaped): ";
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		unsigned char c = buffer[i];
		if (std::isprint(c))
			std::cout << c;
		else
			std::cout << "\\x"
					<< std::hex << std::setw(2) << std::setfill('0') << (int) c
					<< std::dec;
	}
	std::cout << std::endl;
}

// 0. make socket nonblocking
int make_socket_nonblocking(int fd)
{
	//fcntl stands for file control int fcntl(int fd, int cmd, ... /* arg */);
	int current_flags = fcntl(fd, F_GETFL, 0);
	// get the current file status flags for this fd, returns an integer bitmask something like O_RDONLY | O_NONBLOCK | O_APPEND
	// std::cout << "current flags: " << current_flags << std::endl;
	return fcntl(fd, F_SETFL, current_flags | O_NONBLOCK); // add to the current flags the nonblocking flag
}

// 1. creating server socket
int createServerSocket()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // input: IPv4 protocol, TCP socket, default , also returns a fd
	if (serverSocket == -1)
		throw std::runtime_error("Socket creation failed");
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	// reuse port when server gets started to fast
	{
		close(serverSocket);
		throw std::runtime_error("setsockopt failed: " + std::string(strerror(errno)));
	}
	make_socket_nonblocking(serverSocket);
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
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK) // errno gets set by accept, both errnos are the same
			return -1;
		throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
	}
	std::cout << "Client connected." << std::endl;
	return clientSocket;
}

void handleNewConnection(int serverSocket, std::vector<pollfd> &fds)
{
	sockaddr_in clientAddr{};
	int clientSocket = acceptClient(serverSocket, clientAddr);
	if (clientSocket >= 0)
	{
		make_socket_nonblocking(clientSocket);
		pollfd newClient{clientSocket, POLLIN, 0};
		fds.push_back(newClient);
		std::cout << "Client connected (fd=" << clientSocket << ")\n";
	}
}

bool handleClientData(pollfd &client, std::string &buffer)
{
	char recvBuf[512];
	int bytesReceived = recv(client.fd, recvBuf, sizeof(recvBuf) - 1, 0);

	if (bytesReceived == 0)
		return true; // client disconnected

	if (bytesReceived < 0) // this case is for do nothing and wait for data
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			return true; // real error
		return false; // try again later
	}
	recvBuf[bytesReceived] = '\0';
	buffer += recvBuf;
	printEscapedBuffer(buffer);
	size_t end;
	while ((end = buffer.find("\r\n")) != std::string::npos)
	{
		std::string msg = buffer.substr(0, end);
		buffer.erase(0, end + 2);
		std::cout << "Message from " << client.fd << ": " << msg << "\n";
		// handleIRCCommand(message);  // parse and respond
	}
	return false;
}

void closeClient(std::vector<pollfd> &fds, size_t i)
{
	std::cout << "Client " << fds[i].fd << " disconnected\n";
	close(fds[i].fd);
	fds.erase(fds.begin() + i);
}

int main()
{
	uint16_t port = 8080; // typical uint16_t is used for tdp and udp ports
	size_t queue_size = 5;
	int serverSocket = createServerSocket();
	sockaddr_in serverAddress = createServerAddress(port);
	bindServerSocket(serverSocket, serverAddress);
	listenServerSocket(serverSocket, queue_size);

	// Create a vector of pollfd structs
	std::vector<pollfd> fds;

	// Add the server socket to poll list
	pollfd pfd{0, 0, 0}; // set all struct atributes to zero
	pfd.fd = serverSocket;
	pfd.events = POLLIN; // there is data to read
	fds.push_back(pfd);
	std::cout << "Server listening on port " << port << "..." << std::endl;

	int clientSocket;
	std::string buffer;
	while (true)
	{
		// int poll(struct pollfd *fds, nfds_t nfds, int timeout);
		// fds = Pointer to an array of pollfd structs, nfds = Number of entries in the fds array, timeout = Maximum wait time in milliseconds, -1 means indefinitely
		int ready = poll(fds.data(), fds.size(), -1);
		if (ready == -1)
			throw std::runtime_error("Poll failed: " + std::string(strerror(errno)));
		for (size_t i = 0; i < fds.size(); i++)
		{
			bool shouldClose = false;
			// Case 1: The listening socket got a new connection
			if (fds[i].revents & POLLIN)
			// revents check if any revents occurred and pollin signals connection ready to accept, both together means connection is ready or there is fd which has data to read
			{
				if (fds[i].fd == serverSocket)
					handleNewConnection(serverSocket, fds);
				else
					shouldClose = handleClientData(fds[i], buffer);
			}
			if (fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				shouldClose = true;
			if (shouldClose)
				closeClient(fds, i--);
		}
	}
	// close(clientSocket);
	close(serverSocket);
	std::cout <<"closing server connection" << std::endl;
	return 0;
}
