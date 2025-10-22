/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:23:29 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/22 21:49:32 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Nick::Nick(Server *server) : ACommand("NICK", server) {}

void	Nick::parser_debugging(Tokenizer *tokens) const
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
			std::cout	<< tokens->get_params().at(i) << "|";
		std::cout	<< std::endl;
	}
}

void	Nick::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);

	if (tokens->get_params().size() != 1)
	{
		return;
	}
	std::string nickname = tokens->get_param(0);
	if (nickname.empty())
	{
		return;
	}
	if (!is_valid(nickname))
	{
		return;
	}
	client->setNickname(nickname);
}

bool	is_valid(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (!std::isalnum(str[i]) && str[i] != '_')
			return (false);
	}
	return (true);
}