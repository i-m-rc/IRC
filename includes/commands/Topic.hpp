#pragma once

#include "Command.hpp"

class Topic : public Command
{
	private:
		bool exceptionTopic();
		bool printTopic(int chIdx);
		bool checkAuth(int chIdx);
	public:
		Topic(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Topic();

		void execute();
};