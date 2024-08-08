#include "server.hpp"
#include "replies.hpp"
#include "convert.hpp"


// //**************************************************************************************************************


// int Server::check_properties(Channel &channel, std::string mdp, Client *client)
// {
//     if (channel.getmodes().find("k") != std::string::npos) {
//         if (channel.getpassword().compare(mdp) != 0 || mdp.empty())
//         {
//             sendData(client->clientSocket, ERR_BADCHANNELKEY(client->nickname, channel.getName()));
//             return(1);
//         }
//     }
//         if (channel.getmodes().find("l") != std::string::npos && channel.getSize() - channel.getlimit() <= 0)
//         {
//             sendData(client->clientSocket, ERR_CHANNELISFULL(client->nickname, channel.getName()));
//             return(1);
//         }
//         if(channel.getmodes().find("i") != std::string::npos )
//         {
//             if (channel.getinvited().find(client->clientSocket) == channel.getinvited().end())
//             {
//                 sendData(client->clientSocket, ERR_INVITEONLYCHAN(client->nickname, channel.getName()));
//                 return(1);
//             }
//         }
//     return (0);     
// }

// void Server::join(std::string value, Client *client)
// {
//     value = value.substr(value.find(" ") + 1);
//     std::istringstream iss(value);
//     std::map<std::string, std::string> map_channels;
//     std::string channels;
//     std::string ch_name;
//     std::string password;
//     std::string p;
//     std::string ch;
//     std::getline(iss, channels, ' ');
//     std::stringstream ss(strTrim( channels , " ")); 
//         password = value.substr( value.find(" ") + 1,value.size());
//         std::stringstream pa(strTrim( password , " "));
    
//     if (strTrim(value, " ").empty())
//         sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "JOIN"));
//     if(value.find(" ") != std::string::npos)
//     {
//         while (getline(pa, p, ',') && getline(ss, ch, ',') )
//         {
//             if(ch[0] != '#')
//             {
//                 std::cout << ch << std::endl;
//                 sendData(client->clientSocket, ERR_BADCHANMASK(ch));
//                 while((getline(ss, ch, ',') && ch[0] != '#' ));
//             }
//             if(ch[0] == '#')
//             {
//                 map_channels[ch] = p; 
//             }
//         }
//     }

//     while (getline(ss, ch, ','))
//         {
//             if(ch[0] != '#')
//             {
//                 sendData(client->clientSocket, ERR_BADCHANMASK(ch));
//                 while((getline(ss, ch, ',') && ch[0] != '#' ));
//             }
//             if(ch[0] == '#')
//             {
//                 map_channels[ch] = "-1"; 
//             }
//         }
//     std::map<std::string, std::string>::iterator it;
//     std::map<std::string, std::string>::iterator element;
//     // Client *member;
//     std::string msg;
//     for (it = map_channels.begin(); it != map_channels.end(); ++it)
//     {
//         if (server_channels.find(it->first.substr(1)) != server_channels.end())
//         {
//             Channel *channel = server_channels.find(it->first.substr(1))->second;
//             if (check_properties(*channel,  map_channels[it->first], client) == 0)
//             {
//                 if (channel->getmembers().find(client->clientSocket)->first != client->clientSocket)
//                 {
//                     if (channel->inviteonly && channel->getinvited().find(client->clientSocket)->first != client->clientSocket)
//                     {
//                        sendData(client->clientSocket, ERR_INVITEONLYCHAN(serverClients[client->clientSocket]->nickname, channel->getName()));
//                        return;
//                     }
//                     channel->add_user(client, client->clientSocket, 0);
//                     channel->setSize(channel->getSize() + 1);
//                     if (!channel->gettopic().empty())
//                         sendData(client->clientSocket, RPL_TOPIC(serverClients[client->clientSocket]->nickname, channel->getName(), channel->gettopic()));
//                 }
//             }
//             else
//             {
//                 return;
//             }
//         }
//         else 
//         {
//             Channel *channel = new Channel();
//             channel->setName(it->first.substr(1));
//             if (map_channels[it->first].compare("-1") != 0)
//             {
//                 channel->setpassword(map_channels[it->first]);
//                 channel->setmodes("k");
//             }
//                 server_channels.insert(std::make_pair(it->first.substr(1), channel));
//                 channel->add_user(client, client->clientSocket, 0);
//                 channel->add_user(client, client->clientSocket, 1);
//                 channel->setSize(channel->getSize() + 1);
//                 msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + getIp(client->clientAdress) + " JOIN " + "#" +channel->getName();
//                 sendData(client->clientSocket, msg);
//                 msg = ":"+ getIp(client->clientAdress) +" 353 " + serverClients[client->clientSocket]->nickname + " = #" + channel->getName()  +" : "+channel->getMemberNames();
//                 sendData(client->clientSocket, msg);
//                 msg = ":"+ getIp(client->clientAdress) +" 366 " + serverClients[client->clientSocket]->nickname  + " #" + channel->getName() + " :End of /NAMES list" ;
//                 sendData(client->clientSocket, msg);
                
//                 if (!channel->gettopic().empty())
//                     sendData(client->clientSocket, RPL_TOPIC(serverClients[client->clientSocket]->nickname, channel->getName(), channel->gettopic()));
//         }
//     }
            
  
// }


// /************************************************MODE***************************************************************/
// int Server::get_nick(std::string chName, std::string nickname)
// {
//       std::map<int, Client *>::iterator itt;
//     //   (void)nickname;
//     //   std::cout << "no nicknamvve" << std::endl;
//             for (itt = server_channels.find(chName.substr(1))->second->getmembers().begin(); itt != server_channels.find(chName.substr(1))->second->getmembers().end(); ++itt) {
//                 std::cout << itt->first << std::endl;

//                 if (!itt->second->nickname.empty()  && itt->second->nickname  == nickname)
//                 {
//                     return (itt->first);
//                 }
//             }
//             // std::cout << "no nickname" << std::endl;
//             return (-1);
// }

// void Server::send_info(Channel &chName, std::string msg)
// {
//     std::map<int, Client *>::iterator itt;
//     std::cout << chName.getmembers().size() << std::endl;
//     for (itt = chName.getmembers().begin(); itt != chName.getmembers().end(); ++itt)
//     {

//         sendData(itt->first, msg);
//     }

// }


// void send_Mode( int clientsocket, std::map<int, Client >&  serverClients, std::string chname, char mode, std::string sign , int witharg, std::string args)
// {
//             std::map<int, Client >::iterator itt;
//             std::string arg = "";
//             std::string str1(1, mode);;
//             if (witharg != 0)
//                 arg = args;
//             // for (itt = serverClients.begin(); itt != serverClients.end(); ++itt) {
//             //     if (serverClients[clientsocket].clientSocket != clientsocket)
//             //     {
//             //         sendData(clientsocket ,RPL_SENDMODEIS(serverClients[clientsocket].nickname, chname, str1, sign, arg));
//             //     }
//             // }
// }


// void Server::mode(std::string value, Client *client)
// {
// //    (void)clientsocket;
// //    (void)value;
// //    (void)serverClients;
//     std::istringstream iss(value);
//     std::string channel;
//     std::string msg;
//     std::vector<std::string> args;
//     std::vector<std::string> modes;
//     std::string ss;


//     while (std::getline(iss, channel, ' ')) {  
//         if (strTrim( channel , " ") != "")
//         {
//             args.push_back(strTrim( channel , " "));
//         }
//     } 
//         if (!args[0].empty() && args[0][0] == '#' && server_channels.find(args[0].substr(1)) != server_channels.end())
//         {
//             if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
//             {
//                 // sendData(clientsocket, RPL_CHANNELMODEIS(serverClients[clientsocket].nickname, args[0], server_channels.find(args[0].substr(1))->second->getmodes()) );
//                 msg = ":127.0.0.1 324 " + serverClients[client->clientSocket]->nickname + " #" + server_channels.find(args[0].substr(1))->second->getName() +" :"+ server_channels.find(args[0].substr(1))->second->getmodes();
//                 sendData(client->clientSocket, msg);
//             }
//             else if (!args[1].empty() && (args[1][0] == '-' || args[1][0] == '+'))
//             {
//                 int i = 0;
//                 int j = 0;
//                 std::string s;
//                 while (args[1][i] && (args[1][i] == '+' || args[1][i] == '-'))
//                 {
//                     j = i +1 ;
//                     while(args[1][j] && (args[1][j] != '+' && args[1][j] != '-'))
//                         j++;                                                                    //join #c1,#c2,#c3,c4,#c5 1 22 333 44
//                                                                                                 //MODE #c1 +sdjsjf-sfhf 10

//                     s = args[1].substr(i  , j);
//                     modes.push_back(s);
//                     std::cout << s<< std::endl;
//                     i = i+j;
//                 }
//                 i = 0;
//                 while ((size_t)i < modes.size())      
//                 {
//                     j = 1;
//                     while(modes[i][j])
//                     {
//                         if (server_channels.find(args[0].substr(1)) == server_channels.end())
//                             sendData(client->clientSocket, ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
//                         else if(server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) != std::string::npos && isalpha(modes[i][j]) )
//                         {
//                             if (modes[i][0] == '-' && server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first == client->clientSocket)
//                             {
//                                 ss = server_channels.find(args[0].substr(1))->second->getmodes();
//                                 ss.erase(std::remove(ss.begin(), ss.end(), modes[i][j]), ss.end());
//                                 server_channels.find(args[0].substr(1))->second->setmodes(ss);
//                             }
//                         }
//                         else if (server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first != client->clientSocket)
//                                 sendData(client->clientSocket, ERR_CHANOPRIVSNEEDED(serverClients[client->clientSocket]->nickname, args[0]) );
//                         else if (server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) == std::string::npos && modes[i][0] == '+' && server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first == client->clientSocket && isalpha(modes[i][j])) 
//                         {   
//                             if (modes[i][j] == 'l')
//                             {
//                                 if(!args[2].empty())
//                                 {
//                                     if (convert(args[2]) != -1)
//                                     {
//                                         std::cout << "args[2]     "<<cast_int(args[2]) << " |||||| " << std::endl;
//                                         server_channels.find(args[0].substr(1))->second->setlimit(cast_int(args[2]));
//                                         args.erase(std::next(args.begin(), 2));
//                                         ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
//                                         server_channels.find(args[0].substr(1))->second->setmodes(ss);

//                                     }
//                                     // else
//                                     //     std::cout << " Error on limit number"<< std::endl;
//                                 } 
//                                 else
//                                     sendData(client->clientSocket, ERR_NOTENOUGHPARAM(serverClients[client->clientSocket]->nickname));

//                             }
//                             else if (modes[i][j] == 'k')
//                             {
//                                 if ( !args[2].empty())
//                                 {
//                                     server_channels.find(args[0].substr(1))->second->setpassword(args[2]);
//                                     ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
//                                     server_channels.find(args[0].substr(1))->second->setmodes(ss);
//                                         std::cout << "args[2]     "<< args[2] << " |||||| " << std::endl;
//                                 }
//                                 else
//                                     sendData(client->clientSocket, ERR_NOTENOUGHPARAM(serverClients[client->clientSocket]->nickname));
//                             }
//                             else if (modes[i][j] == 't' || modes[i][j] == 'i')
//                             {
//                                 ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
//                                 server_channels.find(args[0].substr(1))->second->setmodes(ss);
//                                 if (modes[i][j] == 't' )
//                                 {
//                                     server_channels.find(args[0].substr(1))->second->otopic = true;
//                                 }
//                                 else
//                                     server_channels.find(args[0].substr(1))->second->inviteonly = true;

//                             }
//                             else if (modes[i][j] == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
//                             {
//                                 server_channels.find(args[0].substr(1))->second->add_user(serverClients[get_nick(args[0], args[2])],get_nick(args[0], args[2]) ,1 );
//                             }


//                         }
//                         j++;
//                     }
//                     i++;
                                                
//                 }
//             }
//             else if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
//             {
//                 // sendData(client->clientSocket, RPL_CHANNELMODEIS(serverClients[client->clientSocket]->nickname, args[0], server_channels.find(args[0].substr(1))->second->getmodes()) );
//                 msg = ":127.0.0.1 324 " + serverClients[client->clientSocket]->nickname + " #" + server_channels.find(args[0].substr(1))->second->getName() +" :"+ server_channels.find(args[0].substr(1))->second->getmodes();
//                 sendData(client->clientSocket, msg);
//             }

//         }
//         else
//         {
//             // std::cout << "error channel doesnt exist "<< args[0]<< std::endl;
//             sendData(client->clientSocket,ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
//             return;
//         }
//     return;
// }


// // void Server::set_mode(std::vector<std::string> & args, char mode, std::map<int, Client >&  serverClients , std::string ss)
// // {
// //     if (mode == 'l')
// //     {
// //         if(!args[2].empty())
// //         {
// //             if (convert(args[2]) != -1)
// //             {
// //                 server_channels.find(args[0].substr(1))->second->setlimit(cast_int(args[2]));
// //                 args.erase(std::next(args.begin(), 2));
// //                 ss = server_channels.find(args[0].substr(1))->second->getmodes() + mode;
// //                 server_channels.find(args[0].substr(1))->second->setmodes(ss);
// //             }
// //         }
// //     }
// //     else if (mode == 'k')
// //     {
// //         if ( !args[2].empty())
// //         {
// //             server_channels.find(args[0].substr(1))->second->setpassword(args[2]);
// //             ss = server_channels.find(args[0].substr(1))->second->getmodes() + mode;
// //             server_channels.find(args[0].substr(1))->second->setmodes(ss);
// //         }
// //     }
// //     else if (mode == 't' || mode == 'i')
// //     {
// //         ss = server_channels.find(args[0].substr(1))->second->getmodes() + mode;
// //         server_channels.find(args[0].substr(1))->second->setmodes(ss);
// //         if (mode == 't' )
// //         {
// //             server_channels.find(args[0].substr(1))->second->otopic = true;
// //         }
// //         else
// //             server_channels.find(args[0].substr(1))->second->inviteonly = true;
// //     }
// //     else if (mode == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
// //     {
// //         server_channels.find(args[0].substr(1))->second->add_user(&serverClients[get_nick(args[0], args[2])],get_nick(args[0], args[2]) ,1 );
// //     }

// // }
     









//**************************************************************************************************************


int Server::check_properties(Channel *channel, std::string mdp, Client *client)
{
    if (channel->getmodes().find("k") != std::string::npos) {
        if (channel->getpassword().compare(mdp) != 0 || mdp.empty())
        {
            sendData(client->clientSocket, ERR_BADCHANNELKEY(client->nickname, channel->getName()));
            return(1);
        }
    }
        if (channel->getmodes().find("l") != std::string::npos && channel->getlimit() - channel->getSize() <= 0)
        {
            std::cout << channel->getlimit() - channel->getSize() << std::endl;
            sendData(client->clientSocket, ERR_CHANNELISFULL(client->nickname, channel->getName()));
            return(1);
        }
        if(channel->getmodes().find("i") != std::string::npos )
        {
            std::cout<<channel->getinvited().find(client->clientSocket)->first << " | " << channel->getinvited().end()->first <<std::endl;
            std::map<int , Client *>::iterator itt;
            for (itt =channel->getinvited().begin(); itt != channel->getinvited().end(); itt++)
                {
                    std::cout << itt->first << " nick name " << itt->second->nickname << std::endl;
                }
            if (channel->getinvited().find(client->clientSocket)->first != client->clientSocket)
            {
                sendData(client->clientSocket, ERR_INVITEONLYCHAN(client->nickname, channel->getName()));
                return(1);
            }
        }
    return (0);     
}


void Server::join(std::string value, Client *client)
{
    value = strTrim(value, "     ");
    std::istringstream iss(value);
    std::map<std::string, std::string> map_channels;
    std::string channels;
    std::string ch_name;
    std::string password;
    std::string p;
    std::string ch;
    std::getline(iss, channels, ' ');
    std::stringstream ss(strTrim( channels , " ")); 
    password = value.substr( value.find(" ") + 1,value.size());
        std::cout<< "password   |"<< password<< std::endl;
        std::cout<< "channels   |"<< channels<< std::endl;
        std::stringstream pa(strTrim( password , " "));
    
    if (strTrim(value, " ").empty())
        sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "JOIN"));
    if(value.find(" ") != std::string::npos)
    {
        while (getline(pa, p, ',') && getline(ss, ch, ',') )
        {
            std::cout << p <<" , " << ch << std::endl;
            if(ch[0] != '#')
            {
                sendData(client->clientSocket, ERR_BADCHANMASK(ch));
                while((getline(ss, ch, ',') && ch[0] != '#' ));
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
                sendData(client->clientSocket, ERR_BADCHANMASK(ch));
                while((getline(ss, ch, ',') && ch[0] != '#' ));
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
                    // if (element->second->inviteonly && element->second->getinvited().find(client->clientSocket)->first != client->clientSocket)
                    // {
                    //     std::cout<<"invite onlyyy" << element->second->inviteonly << "|" << (element->second->getinvited().find(client->clientSocket)->first != client->clientSocket) << std::endl;
                    //    sendData(client->clientSocket, ERR_INVITEONLYCHAN(serverClients[client->clientSocket]->nickname, element->second->getName()));
                    //    return;
                    // }
                    // member = new Client(serverClients[client->clientSocket]);
                    element->second->add_user(client, 0);
                    element->second->setSize(element->second->getSize()+1);
                    msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + getIp(client->clientAdress) + " JOIN " + "#" +element->second->getName()+"\r\n";
                    send_info(element->second, msg);
                    // sendData(client->clientSocket, msg);
                    // sendData(client->clientSocket, RPL_NAMREPLY(serverClients[client->clientSocket]->nickname, element->second->getName(), element->second->getMemberNames()));
                    // sendData(client->clientSocket, RPL_ENDOFNAMES(serverClients[client->clientSocket]->nickname, element->second->getName()));
                    msg = ":"+getIp(client->clientAdress)+" 353 " + serverClients[client->clientSocket]->nickname + " = #" + element->second->getName()  +" :"+ element->second->getMemberNames()+"\r\n";
                    sendData(client->clientSocket, msg);
                    msg = ":"+getIp(client->clientAdress)+" 366 " + serverClients[client->clientSocket]->nickname  + " #" + element->second->getName() + " :End of /NAMES list." +"\r\n";
                    sendData(client->clientSocket, msg);
                    // sendData(client->clientSocket, RPL_CHANNELMODEIS(getIp(client->clientAdress), element->second->getName(), element->second->getmodes()));
                    if (!element->second->gettopic().empty())
                        sendData(client->clientSocket, RPL_TTOPIC(serverClients[client->clientSocket]->nickname,serverClients[client->clientSocket]->username,getIp(client->clientAdress), element->second->getName(), element->second->gettopic()));
                }
            }
            else
            {
                return;
            }
        }
        else 
        {
            Channel *chn = new Channel();
            chn->setName(it->first.substr(1));
            if (map_channels[it->first].compare("-1") != 0)
            {
                chn->setpassword(map_channels[it->first]);
                chn->setmodes("k");
            }
            server_channels.insert (std::make_pair(it->first.substr(1),chn));
            server_channels.find(it->first.substr(1))->second->add_user(client, 1);
            server_channels.find(it->first.substr(1))->second->add_user(client, 0);
            chn->setSize(chn->getSize()+1);
            msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + getIp(client->clientAdress) + " JOIN " + "#" +chn->getName()+"\r\n";
            sendData(client->clientSocket, msg);
            sendData(client->clientSocket, RPL_NAMREPLY(getIp(client->clientAdress), serverClients[client->clientSocket]->nickname, chn->getName(), chn->getMemberNames() ));
            sendData(client->clientSocket, RPL_ENDOFNAMES(getIp(client->clientAdress), serverClients[client->clientSocket]->nickname, chn->getName()));
            if (!chn->gettopic().empty())
                sendData(client->clientSocket, RPL_TTOPIC(serverClients[client->clientSocket]->nickname,serverClients[client->clientSocket]->username,getIp(client->clientAdress), chn->getName(), chn->gettopic()));
                // msg = ":"+getIp(client->clientAdress)+" 353 " + serverClients[client->clientSocket]->nickname + " = #" + chn->getName()  +" :"+chn->getMemberNames()+"\r\n";
                // msg += ":"+getIp(client->clientAdress)+" 366 " + serverClients[client->clientSocket]->nickname  + " #" + chn->getName() + " :End of /NAMES list." +"\r\n";
                // sendData(client->clientSocket, RPL_NAMREPLY(serverClients[client->clientSocket]->nickname, chn->getName(), chn->getMemberNames()));
                // sendData(client->clientSocket, RPL_ENDOFNAMES(serverClients[client->clientSocket]->nickname, chn->getName()));
                // sendData(client->clientSocket, RPL_CHANNELMODEIS(getIp(client->clientAdress), chn->getName(), chn->getmodes()));
        }
    }
            
  
}


/************************************************MODE***************************************************************/
int Server::get_nick(std::string chName, std::string nickname)
{
      std::map<int, Client *>::iterator itt;
    //   (void)nickname;
    //   std::cout << "no nicknamvve" << std::endl;
            for (itt = server_channels.find(chName.substr(1))->second->getmembers().begin(); itt != server_channels.find(chName.substr(1))->second->getmembers().end(); ++itt) {
                std::cout << itt->first << std::endl;

                if (!itt->second->nickname.empty()  && itt->second->nickname  == nickname)
                {
                    return (itt->first);
                }
            }
            // std::cout << "no nickname" << std::endl;
            return (-1);
}

void Server::send_info(Channel *chName, std::string msg)
{
      std::map<int, Client *>::iterator itt;

            for (itt = chName->getmembers().begin(); itt != chName->getmembers().end(); ++itt) {

               sendData(itt->first, msg);
            }

}



void Server::mode(std::string value, Client *client)
{
    std::istringstream iss(value);
    std::string channel;
    std::string msg;
    std::vector<std::string> args;
    std::vector<std::string> modes;
    std::string modestr;
    std::string modeargs;
    std::string ss;
    int k = -1;
    int m = -1;

    while (std::getline(iss, channel, ' '))
    {  
        if (strTrim( channel , " ") != "")
        {
            args.push_back(strTrim( channel , " "));
        }
    }
        if (!args[0].empty() && args[0][0] == '#' && server_channels.find(args[0].substr(1)) != server_channels.end())
        {
            if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
            {
                // sendData(clientsocket, RPL_CHANNELMODEIS(serverClients[clientsocket].nickname, args[0], server_channels.find(args[0].substr(1))->second->getmodes()) );
                msg = ":127.0.0.1 324 " + serverClients[client->clientSocket]->nickname + " #" + server_channels.find(args[0].substr(1))->second->getName() +" +"+ server_channels.find(args[0].substr(1))->second->getmodes()+"\r\n";
                sendData(client->clientSocket, msg);
            }
            else if (!args[1].empty() && (args[1][0] == '-' || args[1][0] == '+'))
            {
                int i = 0;
                int j = 0;
                std::string s;
                while (args[1][i] && (args[1][i] == '+' || args[1][i] == '-'))
                {
                    j = i +1 ;
                    while(args[1][j] && (args[1][j] != '+' && args[1][j] != '-'))
                        j++;                                                                    //join #c1,#c2,#c3,c4,#c5 1 22 333 44
                                                                                                //MODE #c1 +sdjsjf-sfhf 10

                    s = args[1].substr(i  , j);
                    modes.push_back(s);
                    std::cout << s<< std::endl;
                    i = i+j;
                }
                i = 0;
                while ((size_t)i < modes.size())      
                {
                    j = 1;
                    while(modes[i][j])
                    {
                        if (server_channels.find(args[0].substr(1)) == server_channels.end())
                            sendData(client->clientSocket, ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
                        else if(server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) != std::string::npos && isalpha(modes[i][j]) )
                        {
                            if (modes[i][0] == '-' && server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first == client->clientSocket)
                            {
                                ss = server_channels.find(args[0].substr(1))->second->getmodes();
                                if (k == -1)
                                    modestr.append("-");
                                k++;
                                modestr.append(1, modes[i][j]);
                                ss.erase(std::remove(ss.begin(), ss.end(), modes[i][j]), ss.end());
                                server_channels.find(args[0].substr(1))->second->setmodes(ss);
                                // if (modes[i][j] == 'l')
                                //     server_channels.find(args[0].substr(1))->second->limit = -1;
                                // else if (mode[i][j] == 't')
                                //     server_channels.find(args[0].substr(1))->second->protectedTopic = false;
                                // else if (mode[i][j] == 'i')
                                //     server_channels.find(args[0].substr(1))->second->inviteonly = false;
                                // else if (mode[i][j] == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
                                //     removeUser(serverClients[get_nick(args[0], args[2])]);
                            }
                        }
                        else if (server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first != client->clientSocket)
                                sendData(client->clientSocket, ERR_CHANOPRIVSNEEDED(serverClients[client->clientSocket]->nickname, args[0]) );
                        else if (server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) == std::string::npos && modes[i][0] == '+' && server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first == client->clientSocket && isalpha(modes[i][j])) 
                        {   
                             if (m == -1)
                            modestr.append("+");
                            m++;
                            if (modes[i][j] == 'l')
                            {
                                if(!args[2].empty())
                                {
                                    if (convert(args[2]) != -1)
                                    {

                                        modestr.append(1, modes[i][j]);
                                        modeargs.append(" "+args[2]+" ");
                                        std::cout << "args[2]     "<<cast_int(args[2]) << " |||||| " << std::endl;
                                        server_channels.find(args[0].substr(1))->second->setlimit(cast_int(args[2]));
                                        args.erase(args.begin() + 2);
                                        ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
                                        server_channels.find(args[0].substr(1))->second->setmodes(ss);

                                    }
                                    // else
                                    //     std::cout << " Error on limit number"<< std::endl;
                                } 
                                else
                                    sendData(client->clientSocket, ERR_NOTENOUGHPARAM(serverClients[client->clientSocket]->nickname));

                            }
                            else if (modes[i][j] == 'k')
                            {
                                if ( !args[2].empty())
                                {
                                         modestr.append(1, modes[i][j]);
                                        modeargs.append(" "+args[2]+" ");
                                    server_channels.find(args[0].substr(1))->second->setpassword(args[2]);
                                    ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
                                    server_channels.find(args[0].substr(1))->second->setmodes(ss);
                                        args.erase(args.begin() + 2);
                                        std::cout << "args[2]     "<< args[2] << " |||||| " << std::endl;
                                }
                                else
                                    sendData(client->clientSocket, ERR_NOTENOUGHPARAM(serverClients[client->clientSocket]->nickname));
                            }
                            else if (modes[i][j] == 't' || modes[i][j] == 'i')
                            {
                                ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
                                server_channels.find(args[0].substr(1))->second->setmodes(ss);
                                    modestr.append(1, modes[i][j]);
                                        // modeargs.append(" "+args[2]+" ");
                                if (modes[i][j] == 't' )
                                {
                                    server_channels.find(args[0].substr(1))->second->protectedTopic = true;
                                }
                                else{
                                    std::cout << "invite onlyyyy"<< std::endl;
                                    server_channels.find(args[0].substr(1))->second->inviteonly = true;
                                }

                            }
                            else if (modes[i][j] == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
                            {
                                        modestr.append(1, modes[i][j]);
                                        modeargs.append(" "+args[2]+" ");
                                server_channels.find(args[0].substr(1))->second->add_user(serverClients[get_nick(args[0], args[2])],1 );
                            }
                                //  msg = ":127.0.0.1 324 " + serverClients[client->clientSocket]->nickname + " #" + server_channels.find(args[0].substr(1))->second->getName() +" +"+ server_channels.find(args[0].substr(1))->second->getmodes()+"\r\n";
                                //  sendData(client->clientSocket, msg);

                        }
                        j++;
                    }
                    i++;
                                                
                }
            }
            // else if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
            // {
            //     // sendData(client->clientSocket, RPL_CHANNELMODEIS(serverClients[client->clientSocket]->nickname, args[0], server_channels.find(args[0].substr(1))->second->getmodes()) );

            // }
            if (!modestr.empty() && !modeargs.empty())
            {
                msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + getIp(client->clientAdress) + " MODE " + "#" +server_channels.find(args[0].substr(1))->second->getName() +" "+modestr + modeargs +"\r\n";
                send_info(server_channels.find(args[0].substr(1))->second, msg);

            }

        }
        else
        {
            // std::cout << "error channel doesnt exist "<< args[0]<< std::endl;
            sendData(client->clientSocket,ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
            return;
        }
    return;
}
