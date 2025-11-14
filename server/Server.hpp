/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:58:34 by tbui-quo          #+#    #+#             */
/*   Updated: 2025/11/14 13:58:34 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "../commands/ACommand.hpp"
#include "../channel/Channel.hpp"

class ACommand;

class Channel;

class Server
{
private:
	uint16_t							_port;
	std::string							_password;
	std::string							_hostname;
	std::string							_oper_password;
	int 								_server_fd;
	std::vector<pollfd> 				_poll_fds;
	std::vector<Channel *>				_channels;
	std::map<int, Client*> 				_clients;
	std::map<std::string, ACommand*>	_commands;
	bool								_is_running;

	int			createServerSocket();
	sockaddr_in	createServerAddress();
	void		bindServerSocket();
	void		listenServerSocket(size_t backlog);
	void		addClient(int client_fd);
	void		removeClient(int client_fd);
	void		removeIfDisconnected(Client *client, int client_fd, size_t &i);
	void		handlePollEvents();
	void		handleNewConnection();
	void		handleClientEvent(pollfd &entry, size_t &i);
	void		handleAdminInput();

public:
	// Constructors & Destructor
	Server(uint16_t port, std::string password);
	~Server();

	// Getter
	Client					*getClient(int client_fd);
	Client					*getClient(std::string nickname);
	Channel					*getChannel(std::string name);
	std::string				getHostname();
	std::string				getOperPassword();
	std::vector<Channel*>	getJoinedChannelsByClient(Client *client);

	// Member functions
	void onClientMessage(int client_fd, std::string message);
	void response(Client *client, std::string code, std::string message);
	void sendRaw(Client *client, const std::string &raw);
	void addChannel(Channel *channel);
	void removeChannel(const std::string &channelName);
	void removeClientFromServer(Client *client);
	void run();
};



