/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:21:33 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/28 13:05:50 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Kick::Kick(Server *server) : ACommand("KICK", server) {}

void	Kick::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);
	if (!is_registered_full(client) || !has_enough_params(client, tokens, 2))
		return;
	std::string	channel_name = tokens->get_param(0);
	Channel		*channel = this->_server->get_channel(channel_name);
	Client		*target = this->_server->get_client(tokens->get_param(1));
	std::string	reason = tokens->get_params().size() > 2 ? tokens->get_param(2) : "Kicked";

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
	channel->broadcast(client, this->_name, channel->getName(), target->getNickname() + " :" + reason);
	client->request(client, this->_name, channel->getName(), target->getNickname() + " :" + reason);

	channel->removeOpClient(target);
	channel->removeClient(target);
}