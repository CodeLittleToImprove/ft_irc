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

#include "Client.hpp"
#include "../parser/ACommand.hpp"
#include "../channel/Channel.hpp"

class ACommand;

class Server
{
private:
	uint16_t							_port;
	std::string							_password;
	std::string							_hostname;
	int 								_server_fd;
	std::vector<pollfd> 				_poll_fds;
	std::vector<Channel *>				_channels;
	std::map<int, Client*> 				_clients;
	std::map<std::string, ACommand*>	_commands;
	bool								_is_running;

	int createServerSocket();
	sockaddr_in createServerAddress();
	void bindServerSocket();
	void listenServerSocket(size_t backlog);
	void addClient(int client_fd);
	// void addClient(int client_fd, const std::string password);
	void removeClient(int index);
	void removeIfDisconnected(Client *client, int client_fd, size_t &i, const std::string &context);
	void handlePollEvents();
	void handleNewConnection();
	void handleClientEvent(pollfd &entry, size_t &i);
	void handleAdminInput();

public:
	// Constructors & Destructor
	Server(uint16_t port);
	Server(uint16_t port, std::string password);
	Server(const Server& copy); // should be private?
	~Server();

	// Operator overloads
	Server& operator=(const Server& copy); // should be private ?

	// Getter
	Client		*get_client(int client_fd);
	Client		*get_client(std::string nickname);
	Channel		*get_channel(std::string name);
	std::string	get_hostname();

	// Member functions
	void onClientMessage(int client_fd, std::string message);
	void response(Client *client, std::string code, std::string message);
	void add_channel(Channel *channel);
	void run();
};



