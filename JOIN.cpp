#include "server.hpp"
#include "replies.hpp"
#include "convert.hpp"

int Server::check_properties(Channel *channel, std::string mdp, Client *client)
{
    if (channel->getmembers().find(client->clientSocket) != channel->getmembers().end())
        return(1);
    if (channel->getmodes().find("k") != std::string::npos) {
        if (channel->getpassword().compare(mdp) != 0 || mdp.empty())
        {
            sendData(client->clientSocket, ERR_BADCHANNELKEY(client->nickname, channel->getName(), getIp(client->clientAdress)));
            return(1);
        }
    }
        if (channel->getmodes().find("l") != std::string::npos && channel->getlimit() - channel->getSize() <= 0)
        {

            sendData(client->clientSocket, ERR_CHANNELISFULL(client->nickname, channel->getName(), getIp(client->clientAdress)));
            return(1);
        }
        if(channel->getmodes().find("i") != std::string::npos )
        {

            std::map<int , Client *>::iterator itt;
            if (channel->getinvited().find(client->clientSocket)->first != client->clientSocket)
            {
                sendData(client->clientSocket, ERR_INVITEONLYCHAN(client->nickname, channel->getName(), getIp(client->clientAdress)));
                return(1);
            }
        }
    return (0);     
}


void Server::join(std::string value, Client *client)
{
    if (!client->isRegistered)
    {
        sendData(client->clientSocket, ERR_NOTREGISTERED(client->nickname));
        return ;
    }
    value = strTrim(value, " ");
    std::istringstream iss(value);
    std::map<std::string, std::string> map_channels;
    std::string channels;
    std::string ch_name;
    std::string password;
    std::string p;
    std::string ch;
    // int l = 0;
    std::getline(iss, channels, ' ');
    std::stringstream ss(strTrim( channels , " ")); 
        password = value.substr( value.find(" ") + 1,value.size());
        if (password[0] == ':')
            password = strTrim( password , " ").substr(1);
    std::stringstream pa(strTrim( password , " "));
    
    if (strTrim(value, " ").empty())
        sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "JOIN"));
    if(value.find(" ") != std::string::npos)
    {
        while (getline(pa, p, ',') && getline(ss, ch, ',') )
        {

            if(ch[0] != '#')
            {
                if (!ch.empty())
                    sendData(client->clientSocket, ERR_BADCHANMASK(ch)); 
                while((getline(ss, ch, ',') && ch[0] != '#' ))
                {
                    if (!ch.empty())
                        sendData(client->clientSocket, ERR_BADCHANMASK(ch));

                }
            }
            if(ch[0] == '#')
            {
                map_channels[ch] = p;
            }
        }
    }
        while (getline(ss, ch, ','))
        {
            if(ch[0] != '#')
            {
                if (!ch.empty())
                    sendData(client->clientSocket, ERR_BADCHANMASK(ch)); 
                while((getline(ss, ch, ',') && ch[0] != '#' ))
                {
                    if (!ch.empty())
                        sendData(client->clientSocket, ERR_BADCHANMASK(ch)); 

                }
            }
            if(ch[0] == '#')
            {
                map_channels[ch] = "-1"; 
            }
        }
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator element;

    std::string msg;
    for (it = map_channels.begin(); it != map_channels.end(); ++it) {
        std::map<std::string, Channel *>::iterator element = server_channels.find(it->first.substr(1));
        
        if (element != server_channels.end()) {
            if (check_properties( element->second, map_channels[it->first], client) == 0)
            {
                if (element->second->getmembers().find(client->clientSocket)->first != client->clientSocket)
                {
                    element->second->add_user(client, 0);
                    client->joinedChannels.push_back(element->second);
                    element->second->setSize(element->second->getSize() + 1);
                    msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + getIp(client->clientAdress) + " JOIN " + "#" +element->second->getName()+"\r\n";
                    send_info(element->second, msg);
                    msg = ":"+getIp(client->clientAdress)+" 353 " + serverClients[client->clientSocket]->nickname + " = #" + element->second->getName()  +" :"+ element->second->getMemberNames()+"\r\n";
                    sendData(client->clientSocket, msg);
                    msg = ":"+getIp(client->clientAdress)+" 366 " + serverClients[client->clientSocket]->nickname  + " #" + element->second->getName() + " :End of /NAMES list." +"\r\n";
                    sendData(client->clientSocket, msg);

                    if (!element->second->gettopic().empty())
                        sendData(client->clientSocket, RPL_TTOPIC(serverClients[client->clientSocket]->nickname,serverClients[client->clientSocket]->username,getIp(client->clientAdress), element->second->getName(), element->second->gettopic()));
                }
            }
            else
            {
                return;
            }
        }
        else {
            Channel *chn = new Channel();
            chn->setName(it->first.substr(1));
            // if (map_channels[it->first].compare("-1") != 0)
            // {
                // chn->setpassword(map_channels[it->first]);
                // chn->setmodes("k");
            // }
                server_channels.insert (std::make_pair(it->first.substr(1),chn));

                server_channels.find(it->first.substr(1))->second->add_user(client, 1);
                server_channels.find(it->first.substr(1))->second->add_user(client, 0);
                client->joinedChannels.push_back(chn);
                chn->setSize(chn->getSize() + 1);
                msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + getIp(client->clientAdress) + " JOIN " + "#" +chn->getName()+"\r\n";
                sendData(client->clientSocket, msg);

                sendData(client->clientSocket, RPL_NAMREPLY(getIp(client->clientAdress), serverClients[client->clientSocket]->nickname, chn->getName(), chn->getMemberNames() ));

                sendData(client->clientSocket, RPL_ENDOFNAMES(getIp(client->clientAdress), serverClients[client->clientSocket]->nickname, chn->getName()));
                if (!chn->gettopic().empty())
                    sendData(client->clientSocket, RPL_TTOPIC(serverClients[client->clientSocket]->nickname,serverClients[client->clientSocket]->username,getIp(client->clientAdress), chn->getName(), chn->gettopic()));
        }
    }
            
  
}


/************************************************MODE***************************************************************/
int Server::get_nick(std::string chName, std::string nickname)
{
      std::map<int, Client *>::iterator itt;
            for (itt = server_channels.find(chName.substr(1))->second->getmembers().begin(); itt != server_channels.find(chName.substr(1))->second->getmembers().end(); ++itt) {


                if (!itt->second->nickname.empty()  && itt->second->nickname  == nickname)
                {
                    return (itt->first);
                }
            }
            return (-1);
}
