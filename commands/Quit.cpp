/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:58:34 by tbui-quo          #+#    #+#             */
/*   Updated: 2025/11/14 14:07:39 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Quit::Quit(Server *server) : ACommand("QUIT", server) {}

void	Quit::execute(Client *client, Tokenizer *tokens) const
{
	std::string reason = "Bye Bye";
	if (tokens->getParams().size() > 0)
		reason = tokens->getParam(0);
	client->closeConnection("I decided to leave");
	std::vector<Channel*> joined_channels = _server->getJoinedChannelsByClient(client);
	for (size_t i = 0; i < joined_channels.size(); i++)
	{
		Channel *channel = joined_channels[i];
		if (!channel)
			continue;
		channel->broadcast(client, "QUIT", "", reason);
		channel->removeClient(client);
		if (channel->isEmpty())
			_server->removeChannel(channel->getName());
	}
	this->_server->removeClientFromServer(client);
}