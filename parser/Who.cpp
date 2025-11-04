/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:25:57 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/21 19:13:26 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Who::Who(Server *server) : ACommand("WHO", server) {}

void	Who::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);
	if (!is_registered_full(client) || !has_enough_params(client, tokens, 1))
		return;

	std::string channel_name = tokens->get_params()[0];
	Channel *channel = this->_server->get_channel(channel_name);

	if (!hasChannelAndIsInChannel(client, channel, channel_name))
		return;

	std::string flag;
	if (tokens->get_params().size() > 1)
		flag = tokens->get_params()[1];
	else
		flag = "";
	const std::vector<Client*> &clients = channel->getClients();

	for (size_t i = 0; i < clients.size(); ++i)
	{
		Client *member = clients[i];
		// if flag == "o", skip non-operators
		if (flag == "o" && !channel->isChOper(member->getNickname()))
			continue;
		std::string status = "H";
		if (channel->isChOper(member->getNickname()))
			status += "@";
		std::string whoInfo = channel_name + " " +
					member->getUsername() + " " +
					member->getHostname() + " " +
					this->_server->get_hostname() + " " +
					member->getNickname() + " " +
					status + " 0 " + // hopcount
					member->getRealname();
		// std::cout << "who info:" << whoInfo << std::endl;
		this->_server->response(client, RPL_WHOREPLY, whoInfo);
	}
	std::cout << "end of who list" << std::endl;
	this->_server->response(client, RPL_ENDOFWHO, channel_name + " :End of WHO list");
}