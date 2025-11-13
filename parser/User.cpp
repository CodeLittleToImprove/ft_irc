/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:26:46 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/24 21:43:20 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

User::User(Server *server) : ACommand("USER", server) {}

bool	isPrintable(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (!std::isprint(str[i]))
			return (false);
	return (true);
}

bool	hasSpecialChar(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (!std::isalnum(str[i]) && str[i] != '_' && str[i] != '-' && str[i] != '.')
			return (true);
	return (false);
}

void	User::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);
	
	if (client->isRegistered())
	{
		this->_server->response(client, ERR_ALREADYREGISTRED, ":You are already registered");
		return;
	}
	if (!client->hasNickname())
	{
		this->_server->response(client, ERR_NONICKNAMEGIVEN, ":You have to set a nickname before registering");
		return;
	}
	if (!has_enough_params(client, tokens, 4))
		return;
	std::string username = tokens->get_param(0);
	if (hasSpecialChar(username))
	{
		this->_server->response(client, ERR_ERRONEUSUSERNAME, ":Username should only contain letters, numbers, underscores, dashes and dots");
		return;
	}
	std::string realname = tokens->get_param(3);
	if (!isPrintable(realname))
	{
		this->_server->response(client, ERR_ERRONEUSREALNAME, ":Realname should only contain printable characters");
		return;
	}
	client->registerClient(username, realname);
	if (client->isFullyRegistered())
		this->_server->response(client, RPL_WELCOME, ":Welcome to the Internet Relay Network " + client->getNickname() + "!");
	std::cout << username << " registered as " << realname << std::endl;
}
