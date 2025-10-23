/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cprivmsg.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:59:32 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:33:15 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Cprivmsg::Cprivmsg(Server *server) : ACommand("CPRIVMSG", server) {}

void	Cprivmsg::execute(Client *client, Tokenizer *tokens) const
{
	(void) client;
	parser_debugging(tokens);
}