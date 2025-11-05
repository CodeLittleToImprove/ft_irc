/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:25:57 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/21 19:13:26 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Quit::Quit(Server *server) : ACommand("QUIT", server) {}

void	Quit::execute(Client *client, Tokenizer *tokens) const
{

	std::string reason = "Bye Bye";
	if (tokens->get_params().size() > 1)
	{
		reason = tokens->get_params()[1];
		std::cout << "DEBUG Multiple parameter quit message " << reason << std::endl;
	}
	client->closeConnection("I decided to leave");
	std::vector<Channel*> joined_channels = _server->getJoinedChannelsByClient(client);
	for (size_t i = 0; i < joined_channels.size(); i++)
	{
		Channel *channel = joined_channels[i];
		if (!channel) // for the case channel gets not properly deleted for some reason
			continue;
		channel->broadcast(client, "QUIT", "", reason);
		channel->removeClient(client);
	}
	this->_server->removeClientFromServer(client);
}