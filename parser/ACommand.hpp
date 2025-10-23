/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:13:05 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:36:08 by phillymilly      ###   ########.fr       */
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

class ACommand
{
	protected:
		std::string	_name;
		Server		*_server;
	public:

		// Constructors
		ACommand(std::string name, Server *server);

		// Member functions
		void			parser_debugging(Tokenizer *tokens) const;
		virtual void	execute(Client *client, Tokenizer *tokens) const = 0;
};

class Cnotice : public ACommand
{
	private:
	public:
		Cnotice(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class Cprivmsg : public ACommand
{
	private:
	public:
		Cprivmsg(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Info : public ACommand
{
	private:
	public:
		Info(Server *server);
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
		void	execute(Tokenizer *tokens) const;
};

class List : public ACommand
{
	private:
	public:
		List(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class Mode : public ACommand
{
	private:
	public:
		Mode(Server *server);
		void	execute(Tokenizer *tokens) const;
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
		void	execute(Tokenizer *tokens) const;
};

class Oper : public ACommand
{
	private:
	public:
		Oper(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class Pass : public ACommand
{
	private:
	public:
		Pass(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class Privmsg : public ACommand
{
	private:
	public:
		Privmsg(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class Quit : public ACommand
{
	private:
	public:
		Quit(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class Squit : public ACommand
{
	private:
	public:
		Squit(Server *server);
		void	execute(Tokenizer *tokens) const;
};

class User : public ACommand
{
	private:
	public:
		User(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};

class Users : public ACommand
{
	private:
	public:
		Users(Server *server);
		void	execute(Client *client, Tokenizer *tokens) const;
};