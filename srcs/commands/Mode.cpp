#include "Mode.hpp"

Mode::Mode(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Mode::~Mode() {}

bool Mode::handleException()
{
	Database* DB = Database::getInstance();

	if (DB->getAccount(_fd).isActive() != true)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :Activate first!", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 3)
	{
		if (_parsedCommand.size() != 2)
			sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_parsedCommand[1].front() != '#')
		return true;
	Channel& curChannel = DB->getChannel(DB->search(_parsedCommand[1], CHANNEL));
	if (curChannel.isMemberExists(_fd) || curChannel.isAdmin(_fd))
	{
		sendToClient(_fd, "482", _parsedCommand[0] + " :You are not channel operator!!", SERVER);
		return true;
	}
	return false;		
}

void Mode::execute()
{
	if (handleException())
		return ;
	if (checkMode(_parsedCommand[2]) == false)
		return ;
	if (checkParams(_parsedCommand[2]) == false)
		return ;
	Database *DB = Database::getInstance();
	Channel& curChannel = DB->getChannel(DB->search(_parsedCommand[1], CHANNEL));
	chmod(curChannel, _parsedCommand[2]);
}

bool Mode::checkMode(const std::string& mode)
{
	const std::string modeList = "itklo+-";

	for (size_t i = 0; i < mode.length(); i++)
	{
		if (modeList.find(mode[i]) == std::string::npos)
		{
			std::string errMsg = mode + " :is unknown mode char to me for " + _parsedCommand[1];
			sendToClient(_fd, "472", errMsg, SERVER);
			return false;
		}
	}
	return true;
}

bool Mode::checkParams(const std::string& mode)
{
	int opCode;
	int requiredParamsCount = 0;
	int commandParamsSize = _parsedCommand.size() - 3;

	for (size_t i = 0; i < mode.length(); i++)
	{
		if (mode[i] == '+' || mode[i] == '-')
			opCode = mode[i];
		else
		{
			if ((opCode == '+' && (mode[i] == 'k' || mode[i] == 'l' || mode[i] == 'o')) || \
				(opCode == '-' && (mode[i] == 'k' || mode[i] == 'o')))
				requiredParamsCount++;
		}
	}
	if (commandParamsSize != requiredParamsCount)
		return false;
	return true;
}

void Mode::chmod(Channel& channel, const std::string& mode)
{
	int opCode = '+';
	static bool (Mode::*chmodFunc[5])(Channel& channel, const int opCode, const std::string& param) = {&Mode::changeModeI, &Mode::changeModeT, &Mode::changeModeK, &Mode::changeModeL, &Mode::changeModeO};
	const std::string modeList = "itklo";
	int paramIdx = 0;
	bool successChangeMode;
	for (size_t i = 0; i < mode.length(); i++)
	{
		if (mode[i] == '+' || mode[i] == '-')
			opCode = mode[i];
		else
		{
			if ((opCode == '+' && (mode[i] == 'k' || mode[i] == 'l' || mode[i] == 'o')) || \
				(opCode == '-' && (mode[i] == 'k' || mode[i] == 'o')))
				successChangeMode = (this->*chmodFunc[modeList.find(mode[i])])(channel, opCode, _parsedCommand[3 + paramIdx++]);
			else
				successChangeMode = (this->*chmodFunc[modeList.find(mode[i])])(channel, opCode, "");
			if (successChangeMode)
			{
				if (mode[i] == 'o')
					sendToClient(_fd, "325", _parsedCommand[1] + _parsedCommand[2 + paramIdx], SERVER);
				else
					sendToClient(_fd, "324", _parsedCommand[1] + " " + _parsedCommand[2], SERVER);
			}
		}
	}
}

bool Mode::changeModeI(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('i');
	(void) param;
	if (opCode == '+')
	{
		if (pos != std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode += "i";
		channel.setMode(currMode);
	}
	else
	{
		if (pos == std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
	}
	return true;
}

bool Mode::changeModeT(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('t');
	(void) param;
	if (opCode == '+')
	{
		if (pos != std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode += "t";
		channel.setMode(currMode);
	}
	else
	{
		if (pos == std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
	}
	return true;
}

bool Mode::changeModeK(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('k');
	if (opCode == '+')
	{
		if (pos != std::string::npos)
		{
			sendToClient(_fd, "467", _parsedCommand[1] + " :Channel key already set", SERVER);
			return false;
		}
		std::string currMode = channel.getMode();
		currMode += "k";
		channel.setMode(currMode);
		channel.setKey(param);
	}
	else
	{
		if (pos == std::string::npos || channel.getKey() != param)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
		channel.setKey("");
	}
	return true;
}

bool Mode::changeModeL(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('l');
	if (opCode == '+')
	{
		if (pos != std::string::npos)
			return false;
		for (size_t i = 0; i < param.length(); i++)
			if (std::isdigit(param[i]) == 0)
				return false;
		int limit = std::atoi(param.c_str());
		if (channel.getUserCount() > limit || limit < 1)
			return false;
		channel.setLimit(limit);
		std::string currMode = channel.getMode();
		currMode += "l";
		channel.setMode(currMode);
	}
	else
	{
		if (pos == std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
	}
	return true;
}

bool Mode::changeModeO(Channel& channel, const int opCode, const std::string& param)
{
	bool modeO = opCode == '+' ? true : false;
	int res = channel.chopMember(param, modeO);
	if (res < 0)
	{
		sendToClient(_fd, "441", param + " " + _parsedCommand[1] + " :They aren't on that channel", SERVER);
		return false;
	}
	if (res == 0)
		return false;
	return true;
}
