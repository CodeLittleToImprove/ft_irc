/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:23:29 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/24 22:25:09 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Nick::Nick(Server *server) : ACommand("NICK", server) {}

bool	isValid(std::string str)
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
	// parser_debugging(tokens);

	if (!hasEnoughParams(client, tokens, 1))
		return;
	std::string nickname = tokens->getParam(0);
	if (nickname.empty())
	{
		this->_server->response(client, ERR_NONICKNAMEGIVEN, ":No nickname given");
		return;
	}
	if (this->_server->getClient(nickname))
	{
		this->_server->response(client, ERR_NICKNAMEINUSE, ":Nickname already in use");
		return;
	}
	if (!isValid(nickname))
	{
		this->_server->response(client, ERR_ERRONEUSNICKNAME, ":Nickname should only contain letters, numbers and underscores");
		return;
	}
	// need validation check before setting nickname
	client->setNickname(nickname);
}

