/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 18:31:51 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/15 19:29:40 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#include "utils.hpp"

// Constructors
Tokenizer::Tokenizer() {}

Tokenizer::Tokenizer(const Tokenizer &copy) { *this = copy; }

// Destructor
Tokenizer::~Tokenizer() {}

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

// Member functions
void	Tokenizer::_tokenize(std::string message)
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