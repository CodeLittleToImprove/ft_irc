/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:58:34 by tbui-quo          #+#    #+#             */
/*   Updated: 2025/11/14 13:58:34 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Who::Who(Server *server) : ACommand("WHO", server) {}

void	Who::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);
	if (!isRegisteredFull(client) || !hasEnoughParams(client, tokens, 1))
		return;

	std::string channel_name = tokens->getParams()[0];
	Channel *channel = this->_server->getChannel(channel_name);

	if (!hasChannelAndIsInChannel(client, channel, channel_name))
		return;

	std::string flag; // is needed for operator filter
	if (tokens->getParams().size() > 1)
		flag = tokens->getParams()[1];
	else
		flag = "";
	const std::vector<Client*> &clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		Client *member = clients[i];
		// if flag == "o", skip non-operators
		if (flag == "o" && !channel->isChOper(member->getNickname()))
			continue;
		std::string status = "H"; // represents online and active
		if (channel->isChOper(member->getNickname()))
			status += "@";
		std::string whoInfo = channel_name + " " +
					member->getUsername() + " " +
					member->getHostname() + " " +
					this->_server->getHostname() + " " +
					member->getNickname() + " " +
					status + " 0 " + // hopcount not possible thats why static
					member->getRealname();
		this->_server->response(client, RPL_WHOREPLY, whoInfo);
	}
	this->_server->response(client, RPL_ENDOFWHO, channel_name + " :End of WHO list");
}