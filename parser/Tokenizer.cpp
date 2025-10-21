/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 18:31:51 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/21 19:06:20 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#include "../utils/utils.hpp"

// Constructors
Tokenizer::Tokenizer(std:: string message)
{
	std::vector<std::string> tokens = split(message);
	if (tokens.empty())
		return ;

	// prefix
	if (tokens[0][0] == ':')
	{
		this->_prefix = *tokens.begin();
		tokens.erase(tokens.begin());
	}

	// command
	this->_command = *tokens.begin();
	tokens.erase(tokens.begin());
	std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);

	// params
	for (size_t i = 0; i < tokens.size(); i++)
		this->_params.push_back(tokens[i]);

	tokens.clear();
}

// Operator overloads
Tokenizer	&Tokenizer::operator=(const Tokenizer &copy)
{
	if (this != &copy)
	{
		this->_prefix = copy._prefix;
		this->_command = copy._command;
		this->_params = copy._params;
	}
	return (*this);
}

// Getter
std::string	Tokenizer::get_prefix()
{
	return (this->_prefix);
}

std::string Tokenizer::get_command()
{
	return (this->_command);
}

std::vector<std::string> Tokenizer::get_params()
{
	return (this->_params);
}

std::string Tokenizer::get_param(size_t pos)
{
	return(this->_params.at(pos));
}