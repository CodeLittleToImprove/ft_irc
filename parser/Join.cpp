/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:21:05 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/28 12:54:54 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Join::Join(Server *server) : ACommand("JOIN", server) {}

void	Join::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);

	if (!is_registered_full(client) || !has_enough_params(client, tokens, 1))
		return;
	
	std::string channel_name = tokens->get_param(0);
	if (channel_name[0] != '#')
	{
		this->_server->response(client, ERR_NOSUCHCHANNEL, ":incorrect format. Please use '#' in front of the channel name");
		return;
	}
	Channel *channel = this->_server->get_channel(channel_name);
	if (channel)
	{
		if (channel->isInChannel(client))
		{
			this->_server->response(client, ERR_ALREADYONCHANNEL, ":You are already on the channel");
			return;
		}
		if (channel->isInviteOnly() && !channel->isInvited(client))
		{
			this->_server->response(client, ERR_INVITEONLYCHAN, ":You are not invited");
			return;
		}
		if (channel->isKeyRequired())
		{
			if (!has_enough_params(client, tokens, 2))
			{
				this->_server->response(client, ERR_BADCHANNELKEY, ":Channel key is wrong (Key required)");
				return;
			}
			if (!channel->checkKey(tokens->get_param(1))) // channel has password so it expects 2 parameters
			{
				this->_server->response(client, ERR_BADCHANNELKEY, ":Channel key is wrong");
				return;
			}
		}
		if (channel->hasUserLimit() && channel->getUserNum() >= channel->getUserLimit())
		{
			this->_server->response(client, ERR_CHANNELISFULL, ":Channel is full");
			return;
		}
		// case: join channel
		std::cout << "try to join a channel" << std::endl;
		channel->addClient(client);
		channel->changeUserNum("add");
		channel->broadcast(client, "JOIN", channel->getName(), "");
		// Also send JOIN to the joining client
		client->request(client, "JOIN", channel->getName(), "");
	}
	else
	{
		if (!client->isOper())
		{
			this->_server->response(client, ERR_NOPRIVILEGES, ":You don't have permission to create a new channel");
			return;
		}
		// case: create new channel
		channel = new Channel(channel_name, _server->get_hostname());
		this->_server->add_channel(channel);

		channel->addOpClient(client);
		channel->addClient(client);
		channel->changeUserNum("add");
		channel->broadcast(client, "JOIN", channel->getName(), "");
		// Also send JOIN to the joining client
		client->request(client, "JOIN", channel->getName(), "");
	}
	// send topic info
	if (channel->getTopic().empty())
		_server->response(client, RPL_NOTOPIC, channel->getName() + " :No topic is set");
	else
		_server->response(client, RPL_TOPIC, channel->getName() + " :" + channel->getTopic());

	// send name list
	std::string names = channel->getClientNames();
	// std::cout << "namelist of channel: " << names << std::endl;
	this->_server->response(client, RPL_NAMREPLY, "= " + channel->getName() + " :" + names);
	this->_server->response(client, RPL_ENDOFNAMES, channel->getName() +" :End of Names list");
}