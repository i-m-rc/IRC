#include "User.hpp"

User::User(std::map<int, UserInfo> &clients, uintptr_t fd, std::vector<std::string> temp_split)
	: Command(clients, fd, temp_split)
{
	 std::cout << "User constructor called" << std::endl;
}

User::~User()
{
}

void User::execute()
{
	if (_curUser.isPass() == 0 || _curUser.isActive() == 1)
		return ;

	if (_parsedCommand.size() < 5)
	{
		putString(_fd, "Wrong User args.\n");
		return ;
	}
	else
	{
		// <user> <mode> <unused> <realname>
		if (_parsedCommand[4][0] != ':')
		{
			putString(_fd, "Wrong realname args.\n");
			return ;
		}
		_curUser.setUserName(_parsedCommand[1]);
		_curUser.setAuth(_parsedCommand[2] == "0" ? false : true);
		
		std::string realname;
		for (int i = 4; i < (int)_parsedCommand.size(); i++)
			realname += _parsedCommand[i] + " ";
		realname.erase(std::find(realname.begin(), realname.end(), ':'));
		realname.erase(realname.find_last_not_of(" ") + 1);
		_curUser.setRealName(realname);
		_curUser.setActive(1);
		std::string msg = "Set \nUsername : " + _curUser.getUserName();
		msg += "\nrealname : " + _curUser.getRealName() + "\n";
		putString(_fd, msg.c_str());
	}
}