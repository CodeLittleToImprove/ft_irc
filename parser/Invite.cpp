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
	parser_debugging(tokens);

	if (!is_registered_full(client))
		return;
	if (!has_enough_params(client, tokens, 2))
		return;
	
	std::string	channel_name = tokens->get_param(1);
	Channel 	*channel = this->_server->get_channel(channel_name);
	Client		*target = this->_server->get_client(tokens->get_param(0));

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
	if (!channel->isInChannel(target))
	{
		this->_server->response(client, ERR_NOTONCHANNEL, ":" + target->getNickname() + " is not on the channel " + channel_name);
		return;
	}
	channel->inviteClient(target);
	this->_server->response(client, RPL_INVITING, target->getNickname() + " " + channel->getName());
	target->request(client, this->_name, target->getNickname(), channel->getName());
}