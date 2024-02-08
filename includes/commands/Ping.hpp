#pragma once
#include "Server.hpp"
#include "Command.hpp"
#include <netdb.h>

class Ping : public Command
{
	private:
		void pong();
		bool handleException();
	public:
		Ping(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Ping();
		void execute();
};