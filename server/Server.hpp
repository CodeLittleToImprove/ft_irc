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
#include <poll.h> // for pollfds
#include <iomanip> // for setw and setfill

#include "Client.hpp"

class Server
{
	private:
	int _server_fd;
	std::vector<pollfd> _poll_fds;
	std::vector<Client> _clients;

	public:
	void run();
	void addClient(int client_fd);
	void removeClient(int index);
	void handleClient(int index);
};