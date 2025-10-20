/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 18:31:51 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/20 18:30:43 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#include "utils.hpp"

// Constructors
Tokenizer::Tokenizer(std:: string message)
{
	std::vector<std::string> tokens = split(message, " ");

	if (tokens.empty())
		return ;

	// prefix
	if (tokens[0][0] == ':')
	{
		_prefix = *tokens.begin();
		tokens.erase(tokens.begin());
	}

	// command
	_command = to_upper(*tokens.begin());
	tokens.erase(tokens.begin());

	// params
	for (size_t i = 0; i < tokens.size(); i++)
		_params.push_back(tokens[i]);

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
	return (_prefix);
}

std::string Tokenizer::get_command()
{
	return (_command);
}

std::vector<std::string> Tokenizer::get_params()
{
	return (_params);
}

std::string Tokenizer::get_param(size_t pos)
{
	return(_params.at(pos));
}