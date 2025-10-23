/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 18:22:33 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/21 19:01:02 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string> split(const std::string &str)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token)
    {
        if (!token.empty() && token[0] == ':' && tokens.size() > 0)
        {
            std::string rest;
            std::getline(iss, rest);
            token += rest;
            tokens.push_back(token);
            break;
        }

        tokens.push_back(token);
    }
    return tokens;
}

void printEscapedBuffer(const std::string &buffer)
{
	std::cout << "unfiltered buffer (escaped): ";
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		unsigned char c = buffer[i];
		if (std::isprint(c))
			std::cout << c;
		else
			std::cout << "\\x"
					<< std::hex << std::setw(2) << std::setfill('0') << (int) c
					<< std::dec;
	}
	std::cout << std::endl;
}

