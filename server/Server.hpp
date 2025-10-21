#pragma once

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
#include <map>
#include <poll.h> // for pollfds

#include "serverUtils.cpp"
#include "Client.hpp"
#include "../parser/ACommand.hpp"

class ACommand;

class Server
{
private:
	uint16_t							_port;
	std::string							_password;
	int 								_server_fd;
	std::vector<pollfd> 				_poll_fds;
	std::vector<Client> 				_clients;
	std::map<std::string, ACommand*>	_commands;
	bool								_is_running;

	int createServerSocket();
	sockaddr_in createServerAddress();
	void bindServerSocket();
	void listenServerSocket(size_t backlog);
	void addClient(int client_fd);
	void removeClient(int index);
	void handleNewConnection();

public:
	// Constructors & Destructor
	Server(uint16_t port);
	Server(uint16_t port, std::string password);
	Server(const Server& copy); // should be private?
	// ~Server();

	// Operator overloads
	Server& operator=(const Server& copy); // should be private ?

	// Member functions
	void onClientMessage(std::string message);
	void run();
};



