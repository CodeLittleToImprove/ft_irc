/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:42:10 by phillymilly       #+#    #+#             */
/*   Updated: 2025/11/07 00:27:37 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Topic::Topic(Server *server) : ACommand("TOPIC", server) {}

void	Topic::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);
	if (!isRegisteredFull(client) || !hasEnoughParams(client, tokens, 1))
		return;
	
	std::string channel_name = tokens->getParam(0);
	Channel		*channel = this->_server->getChannel(channel_name);

	if (!channel)
	{
		this->_server->response(client, ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
		return;
	}
	if (!channel->isInChannel(client))
	{
		this->_server->response(client, ERR_NOTONCHANNEL, ":You are not on the channel " + channel_name);
		return;
	}
	if (tokens->getParams().size() == 2)
	{
		if (channel->hasRestriction() && !channel->isChOper(client->getNickname()))
		{
			this->_server->response(client,  ERR_CHANOPRIVSNEEDED, ":" + channel_name + " Cannot set topic. You are not a channel operator");
			return;
		}
		std::string	topic = tokens->getParam(1);
		channel->setTopic(topic);
		channel->broadcast(client, this->_name, channel_name, topic);
	}
	else
	{
		if (channel->getTopic().empty())
			this->_server->response(client, RPL_NOTOPIC, channel_name + " :No topic set");
		else
			this->_server->response(client, RPL_TOPIC, channel_name + " " + channel->getTopic());
	}
}