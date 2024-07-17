#include "channel.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <map>

Channel::Channel(){
this->size = -1;
this->limit = -1;
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

// void Channel::join(std::string value)
// {
    // std::istringstream iss(value);
    // std::map<std::string, std::string> map_channels;
    // std::string channels;
    // std::string ch_name;
    // std::string password;
    // std::string p;
    // std::string ch;
    // std::getline(iss, channels, ' ');
    // // std::cout<<"----"<<channels<<std::endl;
    // // std::cout<<"----"<<strTrim( channels , " ")<<std::endl;
    // std::stringstream ss(strTrim( channels , " ")); 
    //     password = value.substr( value.find(" ") + 1,value.size());
    //     std::stringstream pa(strTrim( password , " "));
    // if(value.find(" ") != std::string::npos)
    // {
    //     while (getline(pa, p, ',') && getline(ss, ch, ',') )
    //     {
    //         if(ch[0] != '#')
    //         {
    //             std::cout<<"problem on channel name "<<std::endl;
    //             while((getline(ss, ch, ',') && ch[0] != '#' ));
    //         }
    //         if(ch[0] == '#')
    //         {
    //             map_channels[ch] = p; 
    //         // std::cout << ch <<" : "<< map_channels[ch] << std::endl;
    //         // std::cout << ch <<" ///////////"<< std::endl;

    //         }
    //     }
    // }
    //     while (getline(ss, ch, ','))
    //     {
    //         if(ch[0] != '#')
    //         {
    //             std::cout<<"problem on channel name "<<std::endl;
    //             while((getline(ss, ch, ',') && ch[0] != '#' ));
    //         }
    //         if(ch[0] == '#')
    //         {
    //             map_channels[ch] = "**"; 

    //         }
    //         // std::cout << ch <<" : "<< map_channels[ch] << std::endl;
    //     }
    // std::map<std::string, std::string>::iterator it;
    // std::map<std::string, std::string>::iterator element;
    // for (it = map_channels.begin(); it != map_channels.end(); ++it) {
    //     std::map<std::string, std::string>::iterator element = server_channels.find(it->first);
    //     if (element != server_channels.end()) {
    //         // std::cout << element << std::endl;
    //         std::cout << "Channel exists: " << std::endl;
    //         std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    //     }
    //     else {
    //         std::cout << "ADD Channels " << std::endl;
    //         ch_name = it->first;
    //         server_channels.insert ( std::pair<std::string,std::string>(ch_name.substr(1),it->second) );
    //     }
    // }
    //         std::map<std::string, std::string>::iterator itt;
    //         for (itt = server_channels.begin(); itt != server_channels.end(); ++itt) {
    //             std::cout << itt->first << "|||||||" << itt->second << std::endl;
    //         }
    // // for (it = map_channels.begin(); it != map_channels.end(); ++it) {
    // //     element = std::find(server_channels.begin(), server_channels.end(), it->first);
    // //     if (element != server_channels.end())
    // //     {
    // //         std::cout << "channel doesnt exist: "<< std::endl;
    // //     }
    // //     else
    // //         std::cout << "channel exist: "<< std::endl;
    // //     std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    // // }
 
// }

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

std::map<int, Client> Channel::getmembers()
{
    return (this->members);
}

std::map<int,  Client> Channel::getinvited()
{
        return (invited_clients);
}

std::map<int, Client> Channel::getoperators()
{
    return (operators);
}

void Channel::add_user(Client c, int clientsocket, int type)
{
    if (type == 0 && members.find(clientsocket) == members.end())
    {
            members.insert(std::pair<int, Client>(clientsocket,c));
        std::map<int, Client>::iterator itt;
            for (itt = members.begin(); itt != members.end(); ++itt) {
                std::cout << itt->first << " client num" << std::endl;
            }
    }
    else if(type == 1 && operators.find(clientsocket) == operators.end())
    {
        operators.insert(std::pair<int, Client>(clientsocket,c));
            std::map<int, Client>::iterator itt;
            for (itt = operators.begin(); itt != operators.end(); ++itt) {
                std::cout << itt->first << std::endl;
            }
    }
    else if(type == -1 && invited_clients.find(clientsocket) == invited_clients.end())
    {
        invited_clients.insert(std::pair<int, Client>(clientsocket,c));
    }
    
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