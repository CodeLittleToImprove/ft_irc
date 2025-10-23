#include "Client.hpp"

Client::Client(int client_fd)
	: _client_fd(client_fd), _connected(true), _has_nickname(false),
	  _is_registered(false), _authenticated(false), _password("") {}

// Client::Client(int client_fd, std::string password)
// 	: _client_fd(client_fd), _connected(true), _has_nickname(false),
// 	  _is_registered(false), _authenticated(false), _password("") {} // setting default password not working yet
// Client::~Client()
// {
// 	if (_connected == false)
// 		close(_client_fd); // is this really needed?
// }



std::vector<std::string> Client::readData()
{
	std::vector<std::string> messages;
	char recvBuf[512];
	int bytesReceived = recv(_client_fd, recvBuf, sizeof(recvBuf) - 1, 0);

	if (bytesReceived > 0)
	{
		recvBuf[bytesReceived] = '\0';
		_buffer += recvBuf;
		// printEscapedBuffer(buffer);
		size_t end;
		while ((end = _buffer.find("\r\n")) != std::string::npos)
		{
			std::string msg = _buffer.substr(0, end);
			_buffer.erase(0, end + 2);
			// std::cout << "Message from " << _client_fd << ": " << msg << "\n";
			messages.push_back(msg);
			// handleIRCCommand(message);  // parse and respond this should be done somewhere else
		}
	}
	else if (bytesReceived == 0)
	{
		// Client closed connection gracefully
		std::cout << "Client " << _client_fd << " closed connection." << std::endl;
		_connected = false; // maybe optional
	}
	else if (!(errno == EAGAIN || errno == EWOULDBLOCK))
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
		// add hostname later
		std::string quitMessage = std::string(":") + RPL_QUIT + " QUIT :" + message + CRLF;
		send(_client_fd, quitMessage.c_str(), quitMessage.length(), 0);
		// need client request RPL_QUIT?
		_connected = false;
		// std::cout << _client_fd << " marked as to close" << std::endl;
		shutdown(_client_fd, SHUT_RDWR); // wake poll via POLLHUP
	}
}

int Client::getClient_fd() const
{
	return _client_fd;
}

bool Client::hasNickname() const
{
	return _has_nickname;
}

bool Client::is_registered() const
{
	return _is_registered;
}

bool Client::getConnectedStatus() const
{
	return _connected;
}

std::string Client::getNickname() const
{
	return _nickname;
}

bool	Client::isAuthenticated(void) const
{
	return _authenticated;
}

void Client::setNickname(std::string nickname)
{
	std::cout << "successful set nickname: " << nickname << std::endl;
	this->_nickname = nickname;
	this->_has_nickname= true;
}

void Client::setPassword(const std::string &password)
{
	_password = password;
}


void Client::register_client(std::string username, std::string realname)
{
	this->_username = username;
	this->_realname = realname;
	this->_is_registered = true;
}

void Client::authenticate(std::string password)
{
	std::cout << "Client " << _client_fd << " authenticated." << std::endl;
	std::cout << "password: " << password << std::endl;
	std::cout << "_password: " << _password << std::endl;
	if (password == _password)
		_authenticated = true;
}
