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
	parser_debugging(tokens);

	if (!is_registered_full(client) || !has_enough_params(client, tokens, 2))
		return;

	std::string target_name	= tokens->get_param(0);
	std::string message		= tokens->get_param(1);
	Client		*target = this->_server->get_client(target_name);

	std::cout	<< "TARGET: " + target->getNickname() << std::endl;

	if (target)
		target->request(client, this->_name, target_name, message);
}