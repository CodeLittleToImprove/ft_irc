#pragma once

# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <signal.h> // need for signal cmd
# include <cerrno>
# include <cstdio> // for perror
# include <poll.h> // for pollfd
# include <vector>
# include "../macros/macros.hpp"

class Client
{
	private:
		int			_client_fd;
		// pollfd *_pollEntry;
		bool 		_connected;
		bool		_has_nickname;
		bool		_is_registered;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _hostname;
		std::string _buffer;

	public:

		// Constructors
		explicit Client(int client_fd);
		~Client();

		// Getter
		int			getClient_fd() const;
		bool		hasNickname() const;
		bool		is_registered() const;
		bool		getConnectedStatus() const;
		std::string	getNickname() const;

		// Setter
		void		setNickname(std::string nickname);

		// Member functions
		std::vector<std::string> readData();
		void closeConnection();
		void register_client(std::string username, std::string realname);
		void request(std::string command, std::string target, std::string message);
};