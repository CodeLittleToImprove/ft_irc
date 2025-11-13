#include "Client.hpp"

Client::Client(int client_fd, std::string password)
	: _client_fd(client_fd), _connected(true), _has_nickname(false), _has_password(false),
	_is_registered(false), _authenticated(false), _is_oper(false), _realname(""), _hostname("localhost"), _password(password) {}

std::vector<std::string> Client::readData()
{
	std::vector<std::string> messages;
	char recvBuf[512];
	int bytesReceived = recv(_client_fd, recvBuf, sizeof(recvBuf) - 1, 0);

	if (bytesReceived > 0)
	{
		recvBuf[bytesReceived] = '\0';
		_buffer += recvBuf;
		if (_buffer.size() > buffermaxsize)
		{
			std::cout << "Error: Message to long from client" << _client_fd << ", discard it" << std::endl;
			_buffer.clear();
			return messages;
		}
		size_t end;
		while ((end = _buffer.find("\r\n")) != std::string::npos)
		{
			std::string msg = _buffer.substr(0, end);
			_buffer.erase(0, end + 2);
			messages.push_back(msg);
		}
	}
	else if (bytesReceived == 0) // Client closed connection gracefully
	{
		std::cout << "Client " << _client_fd << " closed connection." << std::endl;
		_connected = false;
	}
	else
	{
		std::cout << "Recv error on client " << _client_fd << ": " << strerror(errno) << std::endl;
		_connected = false;
	}
	return messages;
}

void Client::closeConnection(std::string message)
{
	if (_connected)
	{
		std::string quitMessage = std::string(":") + RPL_QUIT + " QUIT :" + message + CRLF;
		send(_client_fd, quitMessage.c_str(), quitMessage.length(), 0);
		_connected = false;
		shutdown(_client_fd, SHUT_RDWR); // wake poll via POLLHUP
	}
}

int Client::getClientFd() const
{
	return _client_fd;
}

bool Client::hasNickname() const
{
	return _has_nickname;
}

bool Client::hasPassword() const
{
	return _has_password;
}

bool Client::isRegistered() const
{
	return _is_registered;
}

bool Client::isFullyRegistered() const
{
	if (isRegistered() && isAuthenticated())
		return true;
	return false;
}


bool Client::getConnectedStatus() const
{
	return _connected;
}

std::string Client::getNickname() const
{
	return _nickname;
}

std::string Client::getUsername() const
{
	return _username;
}

std::string Client::getRealname() const
{
	return _realname;
}

std::string Client::getHostname() const
{
	return _hostname;
}

std::string	Client::getPassword() const
{
	return _password;
}

bool Client::isAuthenticated() const
{
	return _authenticated;
}

bool Client::isOper() const
{
	return _is_oper;
}

void Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
	this->_has_nickname = true;
}

void Client::setPassword(const std::string &password)
{
	_password = password;
	_has_password = true;
}

void Client::registerClient(std::string username, std::string realname)
{
	this->_username = username;
	this->_realname = realname;
	this->_is_registered = true;
}

// send an IRC message/request from a sender to this client which is used when client to client / client to channel
void Client::request(Client *sender, std::string command, std::string target, std::string message)
{
	std::string sender_str = ":" + sender->_nickname + "!" + sender->_username + "@" + sender->_hostname;
	std::string message_str = 	command == "KICK"	||
								command == "INVITE" ||
								message.empty()		||
								message[0] == ':' ? message : ':' + message;
	std::string request = sender_str + ' ' + command + ' ' + target + ' ' + message_str + CRLF;
	// printEscapedBuffer(request);
	send(this->_client_fd, request.c_str(), request.length(), 0);
}

bool Client::authenticate(std::string password)
{
	if (password == _password)
		_authenticated = true;
	return _authenticated;
}

void Client::giveOper(std::string oper_password, Server *server)
{
	if (oper_password == server->getOperPassword())
		this->_is_oper = true;
}

void Client::takeOper()
{
	if (this->_is_oper)
		this->_is_oper = false;
}
