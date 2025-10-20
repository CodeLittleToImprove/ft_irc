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

#include "Server.hpp"

Server::Server(std::string port, std::string password) : _port(port), _password(password)
{
	this->_commands["CNOTICE"] = new Cnotice();
	this->_commands["CPRIVMSG"] = new Cprivmsg();
	this->_commands["INFO"] = new Info();
	this->_commands["INVITE"] = new Invite();
	this->_commands["JOIN"] = new Join();
	this->_commands["KICK"] = new Kick();
	this->_commands["LIST"] = new List();
	this->_commands["MODE"] = new Mode();
	this->_commands["NAMES"] = new Names();
	this->_commands["NICK"] = new Nick();
	this->_commands["NOTICE"] = new Notice();
	this->_commands["OPER"] = new Oper();
	this->_commands["PASS"] = new Pass();
	this->_commands["QUIT"] = new Quit();
	this->_commands["SQUIT"] = new Squit();
	this->_commands["USER"] = new User();
	this->_commands["USERS"] = new Users();
}

void Server::addClient(int client_fd)
{
	Client tmp(client_fd);
	//The vector now owns its own copy; tmp will be destroyed after this line
	_clients.push_back(tmp);

	// Create and register this client's poll entry
	pollfd pollEntry;
	pollEntry.fd = client_fd;
	pollEntry.events = POLLIN;
	pollEntry.revents = 0;
	_poll_fds.push_back(pollEntry);
}

void Server::removeClient(int index)
{
	// never remove server_fd
	if (index == 0)
		return;

	int clientIndex = index - 1; // fds[0] is server
	Client &client = _clients[clientIndex];

	close(client.getClient_fd());
	// remove the client from the vector
	_clients.erase(_clients.begin() + clientIndex);

	// remove the corresponding pollfd
	_poll_fds.erase((_poll_fds.begin() + index));
}

void Server::handleClient(int index)
{

}

// debug function
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
	int server_fd = socket(AF_INET, SOCK_STREAM, 0); // input: IPv4 protocol, TCP socket, default , also returns a fd
	if (server_fd == -1)
		throw std::runtime_error("Socket creation failed");
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	// reuse port when server gets started to fast
	{
		close(server_fd);
		throw std::runtime_error("setsockopt failed: " + std::string(strerror(errno)));
	}
	make_socket_nonblocking(server_fd);
	return server_fd;
}

// 2. creating serverAddress
sockaddr_in createServerAddress(uint16_t listenPort)
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
	serverAddress.sin_port = htons(listenPort); // converts to network byte order
	serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP
	// inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr); // works similar to INADDR_ANY
	return serverAddress;
}

// 3. bind the socket to a port
void bindServerSocket(int server_fd, const sockaddr_in &serverAddress)
{
	int bindResult = bind(server_fd, (struct sockaddr *) &serverAddress,
	                      sizeof(serverAddress));
	if (bindResult == -1)
		throw std::runtime_error(std::string("Bind failed: ") + std::strerror(errno));
}

// 4. listen for incoming connections
void listenServerSocket(int server_fd, int backlog)
{
	// listening to the assigned socket
	int listenResult = listen(server_fd, backlog); // n = how many pending connections can be queued
	if (listenResult == -1)
		throw std::runtime_error(std::string("Listen failed: ") + std::strerror(errno));
}

// 5. accept a client connection
int acceptClient(int server_fd, sockaddr_in &clientAddress)
{
	socklen_t clientAddressLen = sizeof(clientAddress);
	// accepting client connection
	// accepts and adds you a new socket FD representing one side of an already-established TCP connection.
	int clientSocket = accept(server_fd, (struct sockaddr *) &clientAddress, &clientAddressLen);
	if (clientSocket == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK) // errno gets set by accept, both errnos are the same
			return -1;
		throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
	}
	std::cout << "Client connected." << std::endl;
	return clientSocket;
}

//helper function
void handleNewConnection(int server_fd, std::vector<pollfd> &fds)
{
	sockaddr_in clientAddr{};
	int clientSocket = acceptClient(server_fd, clientAddr);
	if (clientSocket >= 0)
	{
		make_socket_nonblocking(clientSocket);
		pollfd newClient{clientSocket, POLLIN, 0};
		fds.push_back(newClient);
		std::cout << "Client connected (fd=" << clientSocket << ")\n";
	}
}

//helper function
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
	// printEscapedBuffer(buffer);
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

//helper function
void closeClient(std::vector<pollfd> &fds, size_t i)
{
	std::cout << "Client " << fds[i].fd << " disconnected\n";
	close(fds[i].fd);
	fds.erase(fds.begin() + i);
}

int main()
{
	uint16_t listenPort = 8080; // typical uint16_t is used for tdp and udp ports
	size_t queue_size = 5;
	int serverSocket = createServerSocket();
	sockaddr_in serverAddress = createServerAddress(listenPort);
	bindServerSocket(serverSocket, serverAddress);
	listenServerSocket(serverSocket, queue_size);

	// Create a vector of pollfd structs
	std::vector<pollfd> fds;

	// Add the server socket to poll list
	pollfd server_pfd{0, 0, 0}; // set all struct attributes to zero
	server_pfd.fd = serverSocket;
	server_pfd.events = POLLIN; // there is data to read
	fds.push_back(server_pfd);
	std::cout << "Server listening on port " << listenPort << "..." << std::endl;

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
