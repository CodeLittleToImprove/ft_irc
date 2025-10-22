#pragma once

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h> // need for signal cmd
#include <cerrno>
#include <cstdio> // for perror
#include <poll.h> // for pollfd

class Client
{
	private:
		int			_client_fd;
		// pollfd *_pollEntry;
		bool 		_connected;
		bool		_has_nickname;
		std::string _nickName;
		std::string _buffer;

	public:

		// Constructors
		explicit Client(int client_fd);
		~Client();

		// Getter
		int			getClient_fd() const;
		bool		getConnectedStatus() const;
		std::string	getNickname() const;

		// Setter
		void		setNickname(std::string nickname);

		// Member functions
		bool readData();
		void closeConnection();
};