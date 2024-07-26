#include "channel.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <map>


// Channel::Channel(std::map<int, Client>* members,
//                  std::map<int, Client *>* operators,
//                  std::map<int, Client *>* invited_clients)
//     : members(members), operators(operators), invited_clients(invited_clients) {
//         this->size = -1;
//     this->limit = -1;
//     // Other initializations if needed
// }



Channel::Channel(void) {
    this->size = -1;
    this->limit = -1;
    this->members = new std::map<int, Client >();
    this->operators = new std::map<int, Client >();
    this->invited_clients = new std::map<int, Client >();
}
std::string strTrim( std::string s1,  std::string set) {
    size_t start = 0;
    size_t end = s1.length();
    // Find the first character not in 'set' from the start
    while (start < end && set.find(s1[start]) != std::string::npos) {
        start++;
    }

    // Find the first character not in 'set' from the end
    while (end > start && set.find(s1[end - 1]) != std::string::npos) {
        end--;
    }

    // Substring from start to end
    return s1.substr(start, end - start);
}


void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::settype(std::string type)
{
    this->type = type;
}

void Channel::setmodes(std::string modes)
{
    this->modes = modes;
}

void Channel::settopic(std::string topic)
{
    this->topic = topic;
}

void Channel::setpassword(std::string password)
{
    this->password = password;
}

void Channel::setSize(int size)
{
    this->size = size;
}

void Channel::setlimit(int limit)
{
    this->limit = limit;
}

std::string Channel::getName()
{
    return (name);
}

int Channel::getSize()
{
    return (size);
}

int Channel::getlimit()
{
    return (this->limit);
}

std::string Channel::gettype()
{
    return (type);
}

std::string Channel::getmodes()
{
    return (modes);
}

std::string Channel::gettopic()
{
    return (topic);
}

std::string Channel::getpassword()
{
    return (password);
}

std::map<int, Client > * Channel::getmembers()
{
    return (this->members);
}

std::map<int,  Client >  * Channel::getinvited()
{
        return (invited_clients);
}

std::map<int, Client > * Channel::getoperators()
{
    return (operators);
}

void Channel::add_user(Client * c, int clientsocket, int type)
{
    if (type == 0 && members->find(clientsocket) == members->end())
    {
        members->insert(std::make_pair(clientsocket, *c));
    }
    else if(type == 1 && operators->find(clientsocket) == operators->end())
    {
        operators->insert(std::make_pair(clientsocket, *c));
    }
    else if(type == -1 && invited_clients->find(clientsocket) == invited_clients->end())
    {
        invited_clients->insert(std::make_pair(clientsocket, *c));
    }
    
}

std::string Channel::getMemberNames()
{
    std::string memberNames;
    std::string res;
    std::cout << "members: "<< members->size() << std::endl;
    for (std::map<int, Client >::const_iterator it = members->begin(); it != members->end(); ++it) {
        std::string nickname = it->second.nickname;
        if (operators->find(it->first) != operators->end())
        {
            memberNames += "@" + nickname + ",";
        } else {
            memberNames += nickname + " ";
            std::cout<<"nickname == " + nickname <<std::endl;
        }
        res += memberNames;
    }
    return memberNames;
}

std::string UpperCaseStr(std::string s)
{
    int i = 0;
    while (s[i])
    {
        s[i] = toupper(s[i]);
        i++;
    }
    return (s);
}

int valideName(std::string s)
{
    // int i = 1;
    if (s[0] != '#')
        return(0);
    // while (s[i])
    // {
    //     if(s[i])
    //     i++;
    // }
    return (1);
}

