#include "Pass.hpp"

Pass::Pass(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string password)
	: Command(clients, channels, fd, parsedCommand), _password(password) {
		std::cout << "Pass constructor called" << std::endl;
	}

Pass::~Pass()
{
}
void Pass::execute()
{
	if (exceptionPass())
		return ;
	if (_parsedCommand[1] != _password)
		_curUser.denyAccess();
	else
		_curUser.allowAccess();
}

bool Pass::exceptionPass()
{
	if (_curUser.isActive() == true)
	{
		errorToClient("462", _parsedCommand[0], "Unauthorized command (already registered)");
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		errorToClient("461", _parsedCommand[0], "Not enough parameters");
		return true;
	}
	return false;
}