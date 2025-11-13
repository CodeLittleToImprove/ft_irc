/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:26:09 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/28 12:55:22 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Privmsg::Privmsg(Server *server) : ACommand("PRIVMSG", server) {}

void	Privmsg::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);
	if (!isRegisteredFull(client) || !hasEnoughParams(client, tokens, 2))
		return;
	
	std::string target_name = tokens->get_param(0);
	std::string message = tokens->get_param(1);
	Channel *channel = this->_server->get_channel(target_name);
	Client *target = this->_server->get_client(target_name);

	if (!channel && !target)
	{
		this->_server->response(client, ERR_NOSUCHNICK, ":Target-user/-channel not found");
		return;
	}

	if (channel)
	{
		if (!channel->isInChannel(client))
		{
			this->_server->response(client, ERR_NOTONCHANNEL, ":You are not on the channel '" + target_name + "'");
			return;
		}
		channel->broadcast(client, this->_name, target_name, message);
		client->request(client, this->_name, target_name, message);
	}
	else
		target->request(client, this->_name, target_name, message);
}