/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:21:57 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/20 18:22:21 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

List::List(Server *server) : ACommand("LIST", server) {}

void	List::execute(Tokenizer *tokens) const
{
	std::cout	<< "Command " << this->_name << " called!" << std::endl;
	std::cout	<< "Prefix: ";
	if (tokens->get_prefix().empty())
		std::cout	<< "None" << std::endl;
	else
		std::cout	<< tokens->get_prefix() << std::endl;
	std::cout	<< "Command: " << tokens->get_command() << std::endl;
	std::cout	<< "Parameter: ";
	if (tokens->get_params().empty())
		std::cout	<< "None" << std::endl;
	else
	{
		for (size_t i = 0; i < tokens->get_params().size(); i++)
			std::cout	<< tokens->get_params().at(i) << ", ";
		std::cout	<< std::endl;
	}
}