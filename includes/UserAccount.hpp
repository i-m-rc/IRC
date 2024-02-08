#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

class UserAccount{
public:

private:
	bool		_pass;
    bool		_active;
	bool		_root;
    std::string	_nickName;
    std::string	_realName;
    std::string	_userName;
	std::string _hostName;
	std::string _serverName;
	std::vector<int> _channels;

public:
    UserAccount();
    virtual ~UserAccount();

    std::string sendBuffer;
    //std::map<std::string, bool> channels;


    const std::string &getNickName(void) const;
    const std::string &getRealName(void) const;
    const std::string &getUserName(void) const;
	const std::string &getHostName(void) const;
	const std::string &getServerName(void) const;
	const std::vector<int>& getChannels(void) const;

    void setNickName(const std::string &nickName);
    void setRealName(const std::string &realName);
    void setUserName(const std::string &userName);
	void setHostName(const std::string &hostName);
	void setServerName(const std::string &hostName);
	void addChannel(int idx);

    bool isActive(void) const;
	bool isPass(void) const;
	bool isRoot(void) const;
	bool isInChannel(int idx); //TODO

	void activate(void);
	void deactivate(void);

	void allowAccess(void); 
	void denyAccess(void);

	void authorize(void);
	void deauthorize(void);
};
