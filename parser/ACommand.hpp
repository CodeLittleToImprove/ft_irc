/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:13:05 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/20 17:23:40 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include "Tokenizer.hpp"


class ACommand
{
	private:
		std::string	_name;
	public:

		// Constructors & Destructor
		ACommand(std::string name);
		~ACommand();

		// Member functions
		virtual void	execute(Tokenizer *tokens) const = 0;
};

class Cnotice : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Cnotice();
		~Cnotice();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Cprivmsg : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Cprivmsg();
		~Cprivmsg();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Info : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Info();
		~Info();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Invite : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Invite();
		~Invite();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Join : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Join();
		~Join();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Kick : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Kick();
		~Kick();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class List : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		List();
		~List();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Mode : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Mode();
		~Mode();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Names : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Names();
		~Names();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Nick : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Nick();
		~Nick();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Notice : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Notice();
		~Notice();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Oper : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Oper();
		~Oper();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Pass : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Pass();
		~Pass();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Privmsg : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Privmsg();
		~Privmsg();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Quit : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Quit();
		~Quit();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Squit : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Squit();
		~Squit();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class User : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		User();
		~User();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};

class Users : public ACommand
{
	private:
	public:

		// Constructors & Destructor
		Users();
		~Users();

		// Member functions
		void	execute(Tokenizer *tokens) const;
};