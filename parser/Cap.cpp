/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:21:57 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/21 19:12:56 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Cap::Cap(Server *server) : ACommand("CAP", server) {}

void	Cap::execute(Client *client, Tokenizer *tokens) const
{
	(void)tokens;
	this->_server->response(client, RPL_CAP, ":No supported capabilities available");
}