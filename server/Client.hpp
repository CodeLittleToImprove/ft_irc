#pragma once

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h> // need for signal cmd
#include <cerrno>
#include <cstdio> // for perror

class Client
{
private:
	int _client_fd;
	bool _connected;
	std::string _nickName;
	std::string _buffer;

public:
	explicit Client(int client_fd);
	~Client();

	// int getClient_fd() const;
	bool getConnectedStatus() const;
	std::string getNickname() const;

	void readData();
	void sendData(const std::string &msg);
	void closeConnection();


};