#include "server.hpp"
#include "replies.hpp"
#include "convert.hpp"


// /************************************************JOIN***************************************************************/

// int Server::check_properties(Channel channel, std::string mdp, int client->clientSocket, std::map<int, Client >& serverClients)
// {
//     if (channel.getmodes().find("k") != std::string::npos) {
//         if (channel.getpassword().compare(mdp) != 0 || mdp.empty())
//         {
//             sendError(client->clientSocket, ERR_BADCHANNELKEY(clients_Map[clientsocket].nickname, channel.getName()));
//             return(1);
//         }
//     }
//         if (channel.getmodes().find("l") != std::string::npos && channel.getSize() - channel.getlimit() <= 0)
//         {
//             sendError(clientsocket, ERR_CHANNELISFULL(clients_Map[clientsocket].nickname, channel.getName()));
//             return(1);
//         }
//         if(channel.getmodes().find("i") != std::string::npos )
//         {
//             if (channel.getinvited().find(clientsocket) == channel.getinvited()->end())
//             {
//                 sendError(clientsocket, ERR_INVITEONLYCHAN(clients_Map[clientsocket].nickname, channel.getName()));
//                 return(1);
//             }
//         }
//     return (0);     
// }
// void Server::join(std::string value, int clientsocket, std::map<int, Client >& clients_Map)
// {
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
//         sendError(clientsocket, ERR_NEEDMOREPARAMS(clients_Map[clientsocket].nickname, "JOIN"));
//     if(value.find(" ") != std::string::npos)
//     {
//         while (getline(pa, p, ',') && getline(ss, ch, ',') )
//         {
//             if(ch[0] != '#')
//             {
//                 sendError(clientsocket, ERR_BADCHANMASK(ch));
//                 while((getline(ss, ch, ',') && ch[0] != '#' ));
//             }
//             if(ch[0] == '#')
//             {
//                 map_channels[ch] = p; 
//             }
//         }
//     }
//         while (getline(ss, ch, ','))
//         {
//             if(ch[0] != '#')
//             {
//                 sendError(clientsocket, ERR_BADCHANMASK(ch));
//                 while((getline(ss, ch, ',') && ch[0] != '#' ));
//             }
//             if(ch[0] == '#')
//             {
//                 map_channels[ch] = "-1"; 
//             }
//         }
//     std::map<std::string, std::string>::iterator it;
//     std::map<std::string, std::string>::iterator element;
//     Client *member;
//     std::string msg;
//     for (it = map_channels.begin(); it != map_channels.end(); ++it) {
//         std::map<std::string, Channel>::iterator element = server_channels.find(it->first.substr(1));
//         if (element != server_channels.end()) {
//             if (check_properties( element->second,  map_channels[it->first], clientsocket, clients_Map) == 0)
//             {
//                 if (element->second.getmembers().find(clientsocket)->first != clientsocket)
//                 {
//                     member = new Client(clients_Map[clientsocket]);
//                     // member = &clients_Map[clientsocket];
//                     clients_Map[clientsocket].joinedChannels.push_back(element->second);
//                     element->second.add_user(member, clientsocket, 0);
//                     element->second.setSize(element->second.getSize()+1);
//                     msg = ":" + clients_Map[clientsocket].nickname + "!" + clients_Map[clientsocket].username + "@" + "127.0.0.1" + " JOIN " + "#" +element->second.getName();
//                     sendMessage(clientsocket, msg);
//                     sendError(clientsocket, RPL_JOIN(clients_Map[clientsocket].nickname, element->second.getName()));
//                     sendError(clientsocket, RPL_NAMREPLY(clients_Map[clientsocket].nickname, element->second.getName(), element->second.getMemberNames()));
//                     sendError(clientsocket, RPL_ENDOFNAMES(clients_Map[clientsocket].nickname, element->second.getName()));
//                     if (!element->second.gettopic().empty())
//                         sendError(clientsocket, RPL_TOPIC(clients_Map[clientsocket].nickname, element->second.getName(), element->second.gettopic()));
//                 }
//             }
//             else
//             {
//                 return;
//             }
//         }
//         else {
//             Channel chn = Channel();
//             chn.setName(it->first.substr(1));
//             if (map_channels[it->first].compare("-1") != 0)
//             {
//                 chn.setpassword(map_channels[it->first]);
//                 chn.setmodes("k");
//             }
//                 server_channels.insert (std::make_pair(it->first.substr(1),chn));
//                 member = new Client(clients_Map[clientsocket]);
//                 // member = &clients_Map[clientsocket];
//                 clients_Map[clientsocket].joinedChannels.push_back(chn);
//                 server_channels[it->first.substr(1)].add_user(member, clientsocket, 1);
//                 server_channels[it->first.substr(1)].add_user(member, clientsocket, 0);
//                 chn.setSize(chn.getSize()+1);
//                 msg = ":" + clients_Map[clientsocket].nickname + "!" + clients_Map[clientsocket].username + "@" + "127.0.0.1" + " JOIN " + "#" +chn.getName();
//                 sendMessage(clientsocket, msg);
//                 sendError(clientsocket, RPL_NAMREPLY(clients_Map[clientsocket].nickname, chn.getName(), chn.getMemberNames()));
//                 sendError(clientsocket, RPL_ENDOFNAMES(clients_Map[clientsocket].nickname, chn.getName()));
//                 if (!chn.gettopic().empty())
//                     sendError(clientsocket, RPL_TOPIC(clients_Map[clientsocket].nickname, chn.getName(), chn.gettopic()));
//         }
//     }
            
  
// }


// /************************************************MODE***************************************************************/
// // int Server::get_nick(std::string chName, std::string nickname)
// // {
// //       std::map<int, Client >::iterator itt;
// //       (void)nickname;
// //       std::cout << "no nicknamvve" << std::endl;
// //             for (itt = server_channels.find(chName.substr(1))->second.getmembers().begin(); itt != server_channels.find(chName.substr(1))->second.getmembers().end(); ++itt) {
// //                 std::cout << itt->first << std::endl;

// //                 // if (!itt->second.nickname.empty()  && itt->second.nickname  == nickname)
// //                 // {
// //                 //     std::cout<< "first"<< std::endl;
// //                 //     return (itt->first);
// //                 // }
// //             }
// //             // std::cout << "no nickname" << std::endl;
// //             return (-1);
// // }




// // void send_Mode( int clientsocket, std::map<int, Client >&  clients_Map, std::string chname, char mode, std::string sign , int witharg, std::string args)
// // {
// //             std::map<int, Client >::iterator itt;
// //             std::string arg = "";
// //             std::string str1(1, mode);;
// //             if (witharg != 0)
// //                 arg = args;
// //             for (itt = clients_Map.begin(); itt != clients_Map.end(); ++itt) {
// //                 if (clients_Map[clientsocket].clientSocket != clientsocket)
// //                 {
// //                     sendError(clientsocket ,RPL_SENDMODEIS(clients_Map[clientsocket].nickname, chname, str1, sign, arg));
// //                 }
// //             }
// // }


// // void Server::mod(std::string value, int clientsocket, std::map<int, Client >&  clients_Map)
// // {
// //    (void)clientsocket;
// // //    (void)value;
// //    (void)clients_Map;
// //     std::istringstream iss(value);
// //     std::string channel;
// //     std::vector<std::string> args;
// //     std::vector<std::string> modes;
// //     std::string ss;


// //     while (std::getline(iss, channel, ' ')) {  
// //         if (strTrim( channel , " ") != "")
// //         {
// //             args.push_back(strTrim( channel , " "));
// //             // std::cout << "|"<< strTrim( channel , " ") << "|"<< std::endl;
// //         }
// //     } 
// //         std::cout << " fffff : ||||| " << args.size() << "***"  << std::endl;
// //             // std::map<int, Client >::iterator itt;
// //             // // for (itt = server_channels.find(args[0].substr(1))->second.getoperators().begin(); itt != server_channels.find(args[0].substr(1))->second.getoperators().end(); ++itt) {
// //             //     if (server_channels.find(args[0].substr(1))->second.getoperators().find(clientsocket)->first == clientsocket)
// //             //     {
// //             //         std::cout << "*** ***"<< clientsocket<< std::endl;
// //             //         std::cout << "***** "<< server_channels.find(args[0].substr(1))->second.getoperators().find(clientsocket)->first << std::endl;
// //             //     }
// //             // }
// //         //   return ;
// //     // for (size_t i = 1; i < args.size() - 1; ++i) {}
// //         if (!args[0].empty() && args[0][0] == '#' && server_channels.find(args[0].substr(1)) != server_channels.end())
// //         {
// //             if (!args[1].empty() && (args[1][0] == '-' || args[1][0] == '+'))
// //             {
// //                 int i = 0;
// //                 int j = 0;
// //                 std::string s;
// //                 while (args[1][i] && (args[1][i] == '+' || args[1][i] == '-'))
// //                 {
// //                     j = i +1 ;
// //                     while(args[1][j] && (args[1][j] != '+' && args[1][j] != '-'))
// //                         j++;                                                                    //join #c1,#c2,#c3,c4,#c5 1 22 333 44
// //                                                                                                 //MODE #c1 +sdjsjf-sfhf 10

// //                     s = args[1].substr(i  , j);
// //                     modes.push_back(s);
// //                     std::cout << s<< std::endl;
// //                     i = i+j;
// //                 }
// //                 i = 0;
// //                 while ((size_t)i < modes.size())      
// //                 {
// //                     j = 1;
// //                     while(modes[i][j])
// //                     {
// //                         if (server_channels.find(args[0].substr(1)) == server_channels.end())
// //                             sendError(clientsocket, ERR_NOSUCHCHANNEL(clients_Map[clientsocket].nickname, args[0]));
// //                         else if(server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second.getmodes().find(modes[i][j]) != std::string::npos && isalpha(modes[i][j]) )
// //                         {
// //                             if (modes[i][0] == '-' && server_channels.find(args[0].substr(1))->second.getoperators().find(clientsocket)->first == clientsocket)
// //                             {
// //                                 ss = server_channels.find(args[0].substr(1))->second.getmodes();
// //                                 ss.erase(std::remove(ss.begin(), ss.end(), modes[i][j]), ss.end());
// //                                 server_channels.find(args[0].substr(1))->second.setmodes(ss);
// //                                 // std::string arg;
// //                                 // send_Mode( clientsocket,  clients_Map,args[0], modes[i][j],  "-" , 0,arg);
// //                             }
// //                         }
// //                         else if (server_channels.find(args[0].substr(1))->second.getoperators().find(clientsocket)->first != clientsocket)
// //                                 sendError(clientsocket, ERR_CHANOPRIVSNEEDED(clients_Map[clientsocket].nickname, args[0]) );
// //                         else if (server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second.getmodes().find(modes[i][j]) == std::string::npos && modes[i][0] == '+' && server_channels.find(args[0].substr(1))->second.getoperators().find(clientsocket)->first == clientsocket && isalpha(modes[i][j])) 
// //                         {   
// //                             if (modes[i][j] == 'l')
// //                             {
// //                                 if(!args[2].empty())
// //                                 {
// //                                     if (cast_int(args[2]) != -1)
// //                                     {
// //                                         std::cout << "args[2]     "<<cast_int(args[2]) << " |||||| " << std::endl;
// //                                         server_channels.find(args[0].substr(1))->second.setlimit(cast_int(args[2]));
// //                                         args.erase(std::next(args.begin(), 2));
// //                                         ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
// //                                         server_channels.find(args[0].substr(1))->second.setmodes(ss);

// //                                     }
// //                                     else
// //                                         std::cout << " Error on limit number"<< std::endl;
// //                                 } 
// //                                 else
// //                                     std::cout << " Error on number of args"<< std::endl;

// //                             }
// //                             else if (modes[i][j] == 'k')
// //                             {
// //                                 if ( !args[2].empty())
// //                                 {
// //                                     server_channels.find(args[0].substr(1))->second.setpassword(args[2]);
// //                                     ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
// //                                     server_channels.find(args[0].substr(1))->second.setmodes(ss);
// //                                         std::cout << "args[2]     "<< args[2] << " |||||| " << std::endl;
// //                                 }
// //                                 else
// //                                     std::cout <<  " Error on number of args" << std::endl;;
// //                             }
// //                             else if (modes[i][j] == 't' || modes[i][j] == 'i')
// //                             {
// //                                 ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
// //                                 server_channels.find(args[0].substr(1))->second.setmodes(ss);
// //                             }
// //                             else if (modes[i][j] == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
// //                             {
// //                                 std::cout <<  " Error 1" << std::endl;
// //                                 server_channels[args[0].substr(1)].add_user(clients_Map[get_nick(args[0], args[2])], get_nick(args[0], args[2]), 1);
// //                                 std::cout <<  " Error 2" << std::endl;;
// //                                 ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
// //                                 std::cout <<  " Error 3" << std::endl;;
// //                                 server_channels.find(args[0].substr(1))->second.setmodes(ss);                                                                                                                                                                                                                                                                                                                                                                                                  
// //                             }


// //                         }
// //                         j++;
// //                     }
// //                     i++;
                                                
// //                 }
// //             }
// //             else if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
// //                 sendError(clientsocket, RPL_CHANNELMODEIS(clients_Map[clientsocket].nickname, args[0], server_channels.find(args[0].substr(1))->second.getmodes()) );

// //         }
// //         else
// //         {
// //             // std::cout << "error channel doesnt exist "<< args[0]<< std::endl;
// //             sendError(clientsocket,ERR_NOSUCHCHANNEL(clients_Map[clientsocket].nickname, args[0]));
// //             return;
// //         }
// //         std::cout << " after : |||||" << server_channels.find(args[0].substr(1))->second.getmodes() << "||||||" << std::endl;
// //     return;
// // }

//**************************************************************************************************************


int Server::check_properties(Channel channel, std::string mdp, Client *client)
{
    if (channel.getmodes().find("k") != std::string::npos) {
        if (channel.getpassword().compare(mdp) != 0 || mdp.empty())
        {
            sendError(client->clientSocket, ERR_BADCHANNELKEY(client->nickname, channel.getName()));
            return(1);
        }
    }
        if (channel.getmodes().find("l") != std::string::npos && channel.getSize() - channel.getlimit() <= 0)
        {
            sendError(client->clientSocket, ERR_CHANNELISFULL(client->nickname, channel.getName()));
            return(1);
        }
        if(channel.getmodes().find("i") != std::string::npos )
        {
            if (channel.getinvited().find(client->clientSocket) == channel.getinvited().end())
            {
                sendError(client->clientSocket, ERR_INVITEONLYCHAN(client->nickname, channel.getName()));
                return(1);
            }
        }
    return (0);     
}


void Server::join(std::string value, Client *client)
{
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
        std::stringstream pa(strTrim( password , " "));
    
    if (strTrim(value, " ").empty())
        sendError(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "JOIN"));
    if(value.find(" ") != std::string::npos)
    {
        while (getline(pa, p, ',') && getline(ss, ch, ',') )
        {
            if(ch[0] != '#')
            {
                sendError(client->clientSocket, ERR_BADCHANMASK(ch));
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
                sendError(client->clientSocket, ERR_BADCHANMASK(ch));
                while((getline(ss, ch, ',') && ch[0] != '#' ));
            }
            if(ch[0] == '#')
            {
                map_channels[ch] = "-1"; 
            }
        }
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator element;
    // Client *member;
    std::string msg;
    for (it = map_channels.begin(); it != map_channels.end(); ++it) {
        std::map<std::string, Channel>::iterator element = server_channels.find(it->first.substr(1));
        if (element != server_channels.end()) {
            if (check_properties( element->second,  map_channels[it->first], client) == 0)
            {
                if (element->second.getmembers().find(client->clientSocket)->first != client->clientSocket)
                {
                    if (element->second.inviteonly && element->second.getinvited().find(client->clientSocket)->first != client->clientSocket)
                    {
                       sendError(client->clientSocket, ERR_INVITEONLYCHAN(serverClients[client->clientSocket]->nickname, element->second.getName()));
                       return;
                    }
                    // member = new Client(serverClients[client->clientSocket]);
                    element->second.add_user(client, client->clientSocket, 0);
                    element->second.setSize(element->second.getSize()+1);
                    msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + "127.0.0.1" + " JOIN " + "#" +element->second.getName();
                    send_info(element->second, msg);
                    sendMessage(client->clientSocket, msg);
                    // sendError(client->clientSocket, RPL_NAMREPLY(serverClients[client->clientSocket]->nickname, element->second.getName(), element->second.getMemberNames()));
                    // sendError(client->clientSocket, RPL_ENDOFNAMES(serverClients[client->clientSocket]->nickname, element->second.getName()));
                    msg = ":127.0.0.1 353 " + serverClients[client->clientSocket]->nickname + " = #" + element->second.getName()  +" : "+ element->second.getMemberNames();
                    sendMessage(client->clientSocket, msg);
                    msg = ":127.0.0.1 366 " + serverClients[client->clientSocket]->nickname  + " #" + element->second.getName() + " :End of /NAMES list" ;
                    sendMessage(client->clientSocket, msg);
                    if (!element->second.gettopic().empty())
                        sendError(client->clientSocket, RPL_TOPIC(serverClients[client->clientSocket]->nickname, element->second.getName(), element->second.gettopic()));
                }
            }
            else
            {
                return;
            }
        }
        else {
            Channel chn = Channel();
            chn.setName(it->first.substr(1));
            if (map_channels[it->first].compare("-1") != 0)
            {
                chn.setpassword(map_channels[it->first]);
                chn.setmodes("k");
            }
                server_channels.insert (std::make_pair(it->first.substr(1),chn));
                // member = new Client(serverClients[client->clientSocket]);
                server_channels.find(it->first.substr(1))->second.add_user(client, client->clientSocket, 1);
                server_channels.find(it->first.substr(1))->second.add_user(client, client->clientSocket, 0);
                chn.setSize(chn.getSize()+1);
                msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + "127.0.0.1" + " JOIN " + "#" +chn.getName();
                sendMessage(client->clientSocket, msg);
                msg = ":127.0.0.1 353 " + serverClients[client->clientSocket]->nickname + " = #" + chn.getName()  +" : "+chn.getMemberNames();
                sendMessage(client->clientSocket, msg);
                msg = ":127.0.0.1 366 " + serverClients[client->clientSocket]->nickname  + " #" + chn.getName() + " :End of /NAMES list" ;
                sendMessage(client->clientSocket, msg);
                // sendError(client->clientSocket, RPL_NAMREPLY(serverClients[client->clientSocket]->nickname, chn.getName(), chn.getMemberNames()));
                // sendError(client->clientSocket, RPL_ENDOFNAMES(serverClients[client->clientSocket]->nickname, chn.getName()));
                if (!chn.gettopic().empty())
                    sendError(client->clientSocket, RPL_TOPIC(serverClients[client->clientSocket]->nickname, chn.getName(), chn.gettopic()));
        }
    }
            
  
}


/************************************************MODE***************************************************************/
int Server::get_nick(std::string chName, std::string nickname)
{
      std::map<int, Client *>::iterator itt;
    //   (void)nickname;
    //   std::cout << "no nicknamvve" << std::endl;
            for (itt = server_channels.find(chName.substr(1))->second.getmembers().begin(); itt != server_channels.find(chName.substr(1))->second.getmembers().end(); ++itt) {
                std::cout << itt->first << std::endl;

                if (!itt->second->nickname.empty()  && itt->second->nickname  == nickname)
                {
                    return (itt->first);
                }
            }
            // std::cout << "no nickname" << std::endl;
            return (-1);
}

void Server::send_info(Channel chName, std::string msg)
{
      std::map<int, Client *>::iterator itt;

            for (itt = chName.getmembers().begin(); itt != chName.getmembers().end(); ++itt) {

               sendMessage(itt->first, msg);
            }

}


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
//             //         sendError(clientsocket ,RPL_SENDMODEIS(serverClients[clientsocket].nickname, chname, str1, sign, arg));
//             //     }
//             // }
// }


void Server::mod(std::string value, Client *client)
{
//    (void)clientsocket;
//    (void)value;
//    (void)serverClients;
    std::istringstream iss(value);
    std::string channel;
    std::string msg;
    std::vector<std::string> args;
    std::vector<std::string> modes;
    std::string ss;


    while (std::getline(iss, channel, ' ')) {  
        if (strTrim( channel , " ") != "")
        {
            args.push_back(strTrim( channel , " "));
        }
    } 
        if (!args[0].empty() && args[0][0] == '#' && server_channels.find(args[0].substr(1)) != server_channels.end())
        {
            if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
            {
                // sendError(clientsocket, RPL_CHANNELMODEIS(serverClients[clientsocket].nickname, args[0], server_channels.find(args[0].substr(1))->second.getmodes()) );
                msg = ":127.0.0.1 324 " + serverClients[client->clientSocket]->nickname + " #" + server_channels.find(args[0].substr(1))->second.getName() +" :"+ server_channels.find(args[0].substr(1))->second.getmodes();
                sendMessage(client->clientSocket, msg);
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
                            sendError(client->clientSocket, ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
                        else if(server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second.getmodes().find(modes[i][j]) != std::string::npos && isalpha(modes[i][j]) )
                        {
                            if (modes[i][0] == '-' && server_channels.find(args[0].substr(1))->second.getoperators().find(client->clientSocket)->first == client->clientSocket)
                            {
                                ss = server_channels.find(args[0].substr(1))->second.getmodes();
                                ss.erase(std::remove(ss.begin(), ss.end(), modes[i][j]), ss.end());
                                server_channels.find(args[0].substr(1))->second.setmodes(ss);
                            }
                        }
                        else if (server_channels.find(args[0].substr(1))->second.getoperators().find(client->clientSocket)->first != client->clientSocket)
                                sendError(client->clientSocket, ERR_CHANOPRIVSNEEDED(serverClients[client->clientSocket]->nickname, args[0]) );
                        else if (server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second.getmodes().find(modes[i][j]) == std::string::npos && modes[i][0] == '+' && server_channels.find(args[0].substr(1))->second.getoperators().find(client->clientSocket)->first == client->clientSocket && isalpha(modes[i][j])) 
                        {   
                            if (modes[i][j] == 'l')
                            {
                                if(!args[2].empty())
                                {
                                    if (convert(args[2]) != -1)
                                    {
                                        std::cout << "args[2]     "<<cast_int(args[2]) << " |||||| " << std::endl;
                                        server_channels.find(args[0].substr(1))->second.setlimit(cast_int(args[2]));
                                        args.erase(std::next(args.begin(), 2));
                                        ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
                                        server_channels.find(args[0].substr(1))->second.setmodes(ss);

                                    }
                                    // else
                                    //     std::cout << " Error on limit number"<< std::endl;
                                } 
                                else
                                    sendError(client->clientSocket, ERR_NOTENOUGHPARAM(serverClients[client->clientSocket]->nickname));

                            }
                            else if (modes[i][j] == 'k')
                            {
                                if ( !args[2].empty())
                                {
                                    server_channels.find(args[0].substr(1))->second.setpassword(args[2]);
                                    ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
                                    server_channels.find(args[0].substr(1))->second.setmodes(ss);
                                        std::cout << "args[2]     "<< args[2] << " |||||| " << std::endl;
                                }
                                else
                                    sendError(client->clientSocket, ERR_NOTENOUGHPARAM(serverClients[client->clientSocket]->nickname));
                            }
                            else if (modes[i][j] == 't' || modes[i][j] == 'i')
                            {
                                ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
                                server_channels.find(args[0].substr(1))->second.setmodes(ss);
                                if (modes[i][j] == 't' )
                                {
                                    server_channels.find(args[0].substr(1))->second.otopic = true;
                                }
                                else
                                    server_channels.find(args[0].substr(1))->second.inviteonly = true;

                            }
                            else if (modes[i][j] == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
                            {
                                server_channels.find(args[0].substr(1))->second.add_user(serverClients[get_nick(args[0], args[2])],get_nick(args[0], args[2]) ,1 );
                            }


                        }
                        j++;
                    }
                    i++;
                                                
                }
            }
            else if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
            {
                // sendError(client->clientSocket, RPL_CHANNELMODEIS(serverClients[client->clientSocket]->nickname, args[0], server_channels.find(args[0].substr(1))->second.getmodes()) );
                msg = ":127.0.0.1 324 " + serverClients[client->clientSocket]->nickname + " #" + server_channels.find(args[0].substr(1))->second.getName() +" :"+ server_channels.find(args[0].substr(1))->second.getmodes();
                sendMessage(client->clientSocket, msg);
            }

        }
        else
        {
            // std::cout << "error channel doesnt exist "<< args[0]<< std::endl;
            sendError(client->clientSocket,ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
            return;
        }
    return;
}


// void Server::set_mode(std::vector<std::string> & args, char mode, std::map<int, Client >&  serverClients , std::string ss)
// {
//     if (mode == 'l')
//     {
//         if(!args[2].empty())
//         {
//             if (convert(args[2]) != -1)
//             {
//                 server_channels.find(args[0].substr(1))->second.setlimit(cast_int(args[2]));
//                 args.erase(std::next(args.begin(), 2));
//                 ss = server_channels.find(args[0].substr(1))->second.getmodes() + mode;
//                 server_channels.find(args[0].substr(1))->second.setmodes(ss);
//             }
//         }
//     }
//     else if (mode == 'k')
//     {
//         if ( !args[2].empty())
//         {
//             server_channels.find(args[0].substr(1))->second.setpassword(args[2]);
//             ss = server_channels.find(args[0].substr(1))->second.getmodes() + mode;
//             server_channels.find(args[0].substr(1))->second.setmodes(ss);
//         }
//     }
//     else if (mode == 't' || mode == 'i')
//     {
//         ss = server_channels.find(args[0].substr(1))->second.getmodes() + mode;
//         server_channels.find(args[0].substr(1))->second.setmodes(ss);
//         if (mode == 't' )
//         {
//             server_channels.find(args[0].substr(1))->second.otopic = true;
//         }
//         else
//             server_channels.find(args[0].substr(1))->second.inviteonly = true;
//     }
//     else if (mode == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
//     {
//         server_channels.find(args[0].substr(1))->second.add_user(&serverClients[get_nick(args[0], args[2])],get_nick(args[0], args[2]) ,1 );
//     }

// }
     