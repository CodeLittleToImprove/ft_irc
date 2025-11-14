/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:58:34 by tbui-quo          #+#    #+#             */
/*   Updated: 2025/11/14 13:58:34 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include "../macros/macros.hpp"
# include "Server.hpp"
# include "../utils/utils.hpp"

class Server;

class Client
{
	private:
		int			_client_fd;
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
		Client(int client_fd, std::string password);
		// ~Client();

		// Getter
		int			getClientFd() const;
		bool		hasNickname() const;
		bool		hasPassword() const;
		bool		isRegistered() const;
		bool		isFullyRegistered() const;
		bool		getConnectedStatus() const;
		bool		isAuthenticated() const;
		bool		isOper() const;
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getRealname() const;
		std::string getHostname() const;
		std::string	getPassword() const;

		// Setter
		void setNickname(std::string nickname);
		void setPassword(const std::string &password);

		// Member functions
		std::vector<std::string>	readData();
		void						closeConnection(std::string message);
		void						registerClient(std::string username, std::string realname);
		void						request(Client *client, std::string command, std::string target, std::string message);
		bool						authenticate(std::string password);
		void						giveOper(std::string oper_password, Server *server);
		void						takeOper();
};