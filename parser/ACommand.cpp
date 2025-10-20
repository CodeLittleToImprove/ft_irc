/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:12:57 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/20 16:18:57 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

ACommand::ACommand() {}

ACommand::ACommand(std::string name) : _name(name)
{
}

ACommand::ACommand(const ACommand &copy)
{
	*this = copy;
}

ACommand::~ACommand() {}

ACommand	&ACommand::operator=(const ACommand &copy)
{
	return (*this);
}