/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:20:39 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/28 13:05:42 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Invite::Invite(Server *server) : ACommand("INVITE", server) {}

void	Invite::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);

	if (!isRegisteredFull(client))
		return;
	if (!hasEnoughParams(client, tokens, 2))
		return;
	
	std::string	channel_name = tokens->getParam(1);
	Channel 	*channel = this->_server->getChannel(channel_name);
	Client		*target = this->_server->getClient(tokens->getParam(0));

	if (!hasChannelAndIsInChannel(client, channel, channel_name))
		return;
	if (!channel->isChOper(client->getNickname()))
	{
		this->_server->response(client, ERR_CHANOPRIVSNEEDED, ":You are not a channel operator");
		return;
	}
	if (!target)
	{
		this->_server->response(client, ERR_NOSUCHNICK, ":Target-user not found");
		return;
	}
	if (channel->isInChannel(target))
	{
		this->_server->response(client, ERR_NOTONCHANNEL, ":" + target->getNickname() + " is already on the channel " + channel_name);
		return;
	}
	channel->inviteClient(target);
	this->_server->response(client, RPL_INVITING, target->getNickname() + " " + channel->getName());
	target->request(client, this->_name, target->getNickname(), channel->getName());
}