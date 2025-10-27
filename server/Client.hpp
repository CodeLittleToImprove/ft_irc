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
		bool		_has_password;
		bool		_is_registered;
		bool		_authenticated;
		bool		_is_oper;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _hostname;
		std::string _buffer;
		std::string _password;

	public:

		// Constructors
		// Client(int client_fd, std::string password);
		Client(int client_fd, std::string password);
		// Client(int client_fd, std::string password); // not in use right now
		// ~Client();

		// Getter
		int			getClient_fd() const;
		bool		hasNickname() const;
		bool		hasPassword() const;
		bool		is_registered() const;
		bool		getConnectedStatus() const;
		bool		isAuthenticated() const;
		bool		isOper() const;
		std::string	getNickname() const;
		std::string	getPassword() const;

		// Setter
		void		setNickname(std::string nickname);
		void		setPassword(const std::string &password);

		// Member functions
		std::vector<std::string> readData();
		void closeConnection(std::string message);
		void register_client(std::string username, std::string realname);
		void request(Client *client, std::string command, std::string target, std::string message);
		bool authenticate(std::string password);
};