/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:24:00 by pschmunk          #+#    #+#             */
/*   Updated: 2025/11/10 16:45:06 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Notice::Notice(Server *server) : ACommand("NOTICE", server) {}

void	Notice::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);

	if (!isRegisteredFull(client) || !hasEnoughParams(client, tokens, 2))
		return;

	std::string target_name	= tokens->getParam(0);
	std::string message		= tokens->getParam(1);
	Client		*target = this->_server->getClient(target_name);

	std::cout	<< "TARGET: " + target->getNickname() << std::endl;

	if (target)
		target->request(client, this->_name, target_name, message);
}