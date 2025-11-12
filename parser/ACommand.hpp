/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:13:05 by pschmunk          #+#    #+#             */
/*   Updated: 2025/11/10 15:22:30 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include "Tokenizer.hpp"
# include "../server/Server.hpp"
# include "../server/Client.hpp"
# include "../macros/macros.hpp"

class Server;
class Client;
class Channel;

class ACommand
{
	protected:
		std::string	_name;
		Server		*_server;
	public:

		// Constructors
		ACommand(std::string name, Server *server);
		virtual ~ACommand(void);

		// Member functions
		bool			is_registered_full(Client *client) const;
		bool			has_enough_params(Client *client, Tokenizer *tokens, size_t num) const;
		bool			hasChannelAndIsInChannel(Client *client, Channel *channel, std::string channel_name) const;
		void			parser_debugging(Tokenizer *tokens) const;
		virtual void	execute(Client *client, Tokenizer *tokens) const = 0;
};

class Cap : public ACommand
{
	private:
	public:
		Cap(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Invite : public ACommand
{
	private:
	public:
		Invite(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Join : public ACommand
{
	private:
	public:
		Join(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Kick : public ACommand
{
	private:
	public:
		Kick(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Mode : public ACommand
{
	private:
	public:
		Mode(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Names : public ACommand
{
	private:
	public:
		Names(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class Nick : public ACommand
{
	private:
	public:
		Nick(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Notice : public ACommand
{
	private:
	public:
		Notice(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Oper : public ACommand
{
	private:
	public:
		Oper(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Part : public ACommand
{
private:
public:
	Part(Server *server);
	void	execute(Client *client, Tokenizer *tokens) const;
};

class Pass : public ACommand
{
	private:
	public:
		Pass(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Ping : public ACommand
{
private:
public:
	Ping(Server *server);
	void	execute(Client *client, Tokenizer *tokens) const;
};

class Privmsg : public ACommand
{
	private:
	public:
		Privmsg(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Quit : public ACommand
{
	private:
	public:
		Quit(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class User : public ACommand
{
	private:
	public:
		User(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Who : public ACommand
{
	private:
	public:
		Who(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Topic : public ACommand
{
	private:
	public:
		Topic(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};