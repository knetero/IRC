#include "channel.hpp"
#include <string>
<<<<<<< HEAD
=======

>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
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



<<<<<<< HEAD
Channel::Channel(void)
{
    this->size = 0;
    this->limit = -1;
    this->hasTopic = false;
    this->protectedTopic = false;
}

=======
Channel::Channel(void) {
    this->size = -1;
    this->limit = -1;
    this->members = new std::map<int, Client >();
    this->operators = new std::map<int, Client >();
    this->invited_clients = new std::map<int, Client >();
    this->inviteonly = false;
    this->otopic = false;
}
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
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

<<<<<<< HEAD
std::string &Channel::gettopic()
=======
std::string Channel::gettopic()
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
{
    return (topic);
}

std::string Channel::getpassword()
{
    return (password);
}

<<<<<<< HEAD
std::map<int, Client * >  &Channel::getmembers()
=======
std::map<int, Client > * Channel::getmembers()
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
{
    return (this->members);
}

<<<<<<< HEAD
std::map<int,  Client  *>   &Channel::getinvited()
{
    return (invited_clients);
}

std::map<int, Client * >  &Channel::getoperators()
=======
std::map<int,  Client >  * Channel::getinvited()
{
        return (invited_clients);
}

std::map<int, Client > * Channel::getoperators()
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
{
    return (operators);
}

void Channel::add_user(Client * c, int clientsocket, int type)
{
<<<<<<< HEAD
    if (type == 0 && members.find(clientsocket) == members.end())
    {
        members.insert(std::make_pair(clientsocket, c));
    }
    else if(type == 1 && operators.find(clientsocket) == operators.end())
    {
        operators.insert(std::make_pair(clientsocket, c));
    }
    else if(type == -1 && invited_clients.find(clientsocket) == invited_clients.end())
    {
        invited_clients.insert(std::make_pair(clientsocket, c));
=======
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
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
    }
    
}

<<<<<<< HEAD
std::string Channel::getMemberNames()
{
       std::string memberNames;
    
    for (std::map<int, Client *>::iterator it = members.begin(); it != members.end(); ++it) {
        if (operators.find(it->first) != operators.end()) {
            memberNames += "@" + it->second->nickname + " ";
        }
    }

    for (std::map<int, Client *>::iterator it = members.begin(); it != members.end(); ++it) {
        if (operators.find(it->first) == operators.end()) {
            memberNames += it->second->nickname + " ";
        }
    }
    if (!memberNames.empty() && memberNames[memberNames.size() - 1] == ' ')
    {
        memberNames = memberNames.substr(0, memberNames.size() - 1);
=======
// std::string Channel::getMemberNames()
// {
//     std::string memberNames = "";
//     std::string res;
//     for (std::map<int, Client >::const_iterator it = members->begin(); it != members->end(); ++it) {
//         std::string nickname = it->second.nickname;
//         if (operators->find(it->first) != operators->end()) {
//             memberNames += "@" + nickname + " ";
//             res.insert (0,  memberNames);
//         } else {
//             memberNames += nickname + " ";
//             res.insert (res.length(),     memberNames);
//         }
//     }
//     return res;
// }

std::string Channel::getMemberNames()
{
    std::string memberNames;
    
    for (std::map<int, Client>::const_iterator it = members->begin(); it != members->end(); ++it) {
        if (operators->find(it->first) != operators->end()) {
            memberNames += "@" + it->second.nickname + " ";
        }
    }

    for (std::map<int, Client>::const_iterator it = members->begin(); it != members->end(); ++it) {
        if (operators->find(it->first) == operators->end()) {
            memberNames += it->second.nickname + " ";
        }
    }
    if (!memberNames.empty() && memberNames.back() == ' ') {
        memberNames.pop_back();
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
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

<<<<<<< HEAD
////

int Channel::clientExist(std::string name)
{
    std::map<int, Client *>::iterator it;
    for (it = getmembers().begin(); it != getmembers().end(); it++)
    {
        if (it->second->nickname == name)
            return (1);
    }
    return (-1);
}

int Channel::removeUser(Client *client)
{
    std::map<int, Client *>::iterator it;
    for (it = this->getmembers().begin(); it != this->getmembers().end(); it++)
    {
        if (it->second == client)
        {
            this->getmembers().erase(it);
        }
    }

    std::map<int, Client *>::iterator it1;
    for (it1 = this->getoperators().begin(); it1 != this->getoperators().begin(); it1++)
    {
        if (it1->second == client)
            this->getoperators().erase(it1);
    }

    std::map<int, Client *>::iterator it2;
    for (it2 = this->getinvited().begin(); it2 != this->getinvited().end(); it2++)
    {
        if (it2->second == client)
            this->getinvited().erase(it2);
    }

    return (0);
}
=======
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
