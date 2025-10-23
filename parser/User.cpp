/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:26:46 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:31:50 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

User::User(Server *server) : ACommand("USER", server) {}

bool	is_printable(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (!std::isprint(str[i]))
			return (false);
	return (true);
}

static bool	is_valid(std::string str) // added static to prevent compiler error
{
	for (size_t i = 0; i < str.length(); i++)
		if (!std::isalnum(str[i]) && str[i] != '_' && str[i] != '-' && str[i] != '.')
			return (false);
	return (true);
}

void	User::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);
	
	if (client->is_registered())
	{
		this->_server->response(client->getClient_fd(), ERR_ALREADYREGISTRED, ":You are already registered");
		return;
	}
	if (!client->hasNickname())
	{
		this->_server->response(client->getClient_fd(), ERR_NONICKNAMEGIVEN, ":You have to set a nickname before registering");
		return;
	}
	if (tokens->get_params().size() != 4) 
	{
		this->_server->response(client->getClient_fd(), ERR_NEEDMOREPARAMS, ":Wrong number of parameters (Please use ':' in front of the realname)");
		return;
	}
	std::string username = tokens->get_param(0);
	if (!is_valid(username))
	{
		this->_server->response(client->getClient_fd(), ERR_ERRONEUSUSERNAME, ":Username should only contain letters, numbers, underscores, dashes and dots");
		return;
	}
	std::string realname = tokens->get_param(3);
	if (!is_printable(realname))
	{
		this->_server->response(client->getClient_fd(), ERR_ERRONEUSREALNAME, ":Realname should only contain printable characters");
		return;
	}
	client->register_client(username, realname);
	this->_server->response(client->getClient_fd(), RPL_WELCOME, ":Welcome to the Internet Relay Network " + client->getNickname() + "!");
}
