/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:20:39 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:33:50 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Invite::Invite(Server *server) : ACommand("INVITE", server) {}

void	Invite::execute(Client *client, Tokenizer *tokens) const
{
	(void) client;
	parser_debugging(tokens);
}