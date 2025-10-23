/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:23:29 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:29:16 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Nick::Nick(Server *server) : ACommand("NICK", server) {}

bool	is_valid(std::string str) // muss woanders hin da du das auch in user benutzt?
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isalnum(str[i]) && str[i] != '_')
			return (false);
	}
	return (true);
}

void	Nick::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);

	if (tokens->get_params().size() != 1)
	{
		this->_server->response(client->getClient_fd(), ERR_ERRONEUSNICKNAME, ":Wrong number of parameters");
		return;
	}
	std::string nickname = tokens->get_param(0);
	if (nickname.empty())
	{
		this->_server->response(client->getClient_fd(), ERR_NONICKNAMEGIVEN, ":No nickname given");
		return;
	}
	if (this->_server->get_client(nickname))
	{
		this->_server->response(client->getClient_fd(), ERR_NICKNAMEINUSE, ":Nickname already in use");
		return;
	}
	if (!is_valid(nickname))
	{
		this->_server->response(client->getClient_fd(), ERR_ERRONEUSNICKNAME, ":Nickname should only contain letters, numbers and underscores");
		return;
	}
	client->setNickname(nickname);
}

