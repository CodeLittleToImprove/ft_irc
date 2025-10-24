/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:21:05 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/24 22:23:09 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Join::Join(Server *server) : ACommand("JOIN", server) {}

void	Join::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);

	if (!(client->isAuthenticated() && client->is_registered()))
	{
		this->_server->response(client, ERR_NOTREGISTERED, ":You are not registered yet");
		return;
	}
	if (has_enough_params(client, tokens, 1))
		return;
	std::string channel_name = tokens->get_param(0);
	if (channel_name[0] != '#')
	{
		this->_server->response(client, ERR_NOSUCHCHANNEL, ":incorrect format. Please use '#' in front of the channel name");
		return;
	}
	if (Channel *channel = this->_server->get_channel(channel_name))
	{
		if (channel->isInChannel(client))
		{
			this->_server->response(client, ERR_ALREADYONCHANNEL, ":You are already on the channel");
			return;
		}
		client->request(client, this->_name, channel->getName(), "");
		channel->addClient(client);
	}
	else
	{
		if (!client->isOper())
		{
			this->_server->response(client, ERR_NOPRIVILEGES, ":You don't have permission to create a new channel");
			return;
		}
		this->_server->response(client, ERR_NOSUCHCHANNEL, ":Creating channel '" + channel_name + "'");
		channel = new Channel(channel_name, _server->get_hostname());
		this->_server->add_channel(channel);
		client->request(client, this->_name, channel->getName(), "");
		channel->addOpClient(client);
	}
}