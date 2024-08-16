#include "server.hpp"

#ifndef REPLIES_HPP
#define REPLIES_HPP

#define HOSTNAME ":" + Server::getHostName()    
#define CRLF "\r\n"
// #define ERROR(p1) "ERROR :" + p1 + "\r\n"

#define PRIVMSG(p1, p2, p3, p4, p5) ":" + p1 + "!~" + p2 + "@" + p3 + " PRIVMSG " + p4 + " :" + p5 + "\r\n"
#define NICK(p1, p2, p3, p4) ":" + p1 + "!~" + p2 + "@" + p3 + " NICK " + p4 + "\r\n"
#define KICK(p1, p2, p3, p4, p5, p6) ":" + p1 + "!~" + p2 + "@" + p3 + " KICK " + p4 + " " + p5 + " :" + p6 + "\r\n"
#define INVITE(p1, p2, p3, p4, p5) ":" + p1 + "!~" + p2 + "@" + p3 + " INVITE " + p4 + " " + p5 + "\r\n"
#define TOPIC(p1, p2, p3, p4, p5) ":" + p1 + "!~" + p2 + "@" + p3 + " TOPIC " + p4 + " :" + p5 + "\r\n"
#define PART(nickname, username, adress, channel, messgae) ":" + nickname + "!~" + username + "@" + adress + " PART " + channel + " " + message + "\r\n"
#define QUIT(nickname, username, adress, messgae) ":" + nickname + "!~" + username + "@" + adress + " QUIT " + " " + message + "\r\n"
#define ERROR(reason) "ERROR :" + reason + "\r\n"

#define RPL_CHANNELMODEIS(nickname, channelname, modestring) (":"+ nickname + " MODE #" + channelname +  " +" + modestring + CRLF)
#define RPL_SENDMODEIS(nickname, channelname, modestring, sign , arg) ( nickname + " MODE " + channelname +  " : " + sign +  modestring + arg + CRLF)
#define RPL_JOIN(nickname, channelname ) ( nickname + " is joining the channel " + channelname  +  CRLF)
#define RPL_TTOPIC(client,username, channel,adress, topic)  ":" + client + "!" + username + "@" + adress + " TOPIC " + channel + " :" + topic +"\r\n"

#define RPL_WELCOME(p1, p2, p3)                                             HOSTNAME + " 001 " + p1 + " :Welcome to the Internet Relay Network " + p1 + "!~" + p2 + "@" + p3 + "\r\n"
#define RPL_YOURHOST(p1, p2)                                                HOSTNAME + " 002 " + p1 + " :Your host is " + p2 + ", running version 1.0\r\n"
#define RPL_CREATED(p1, p2)                                                 HOSTNAME + " 003 " + p1 + " :This server has been started " + p2 + "\r\n"
#define RPL_NOTOPIC(client, channel)                                        HOSTNAME + " 331 " + client + " " + channel + " No topic is set\r\n" 
#define RPL_TOPIC(client, channel, topic)                                   HOSTNAME + " 332 " + client + " " + channel + " " + topic + "\r\n"
#define RPL_TOPICWHOTIME(client, channel, nick, username, adress, setat)    HOSTNAME + " 333 " + client + " " + channel + " " + nick + "!" + username + "@" + adress + " " + setat + "\r\n"
#define RPL_INVITING(client, nick, channel)                                 HOSTNAME + " 341 " + client + " " + nick + " " + channel + "\r\n"
#define RPL_NAMREPLY(adress,nickname, channelname, names )                  (HOSTNAME + " 353 " + nickname + " = #" + channelname  +" :"+names+"\r\n")
#define RPL_ENDOFNAMES(adress, nickname, channelname )                      (HOSTNAME + " 366 " + nickname  + " #" + channelname + " :End of /NAMES list." +"\r\n")
#define RPL_NICKCHANGE(oldnickname, nickname) (oldnickname + " changed his nickname to " + nickname + "."+ CRLF)
#define ERR_NOSUCHNICK(client, nick)                                        HOSTNAME + " 401 " + client + " " + nick + " :No such nick" + CRLF 
#define ERR_NOSUCHCHANNEL(client, channel)                                  HOSTNAME + " 403 " + client + " " + channel +  " :No such channel" + CRLF
#define ERR_CANNOTSENDTOCHAN(p1, p2)                                        HOSTNAME + " 404 " + p1 + " " + p2 + " :Cannot send to channel\r\n"
#define ERR_NOTEXTTOSEND(nickname)                                          (HOSTNAME + " 412 " + nickname + " :No text to send" + CRLF)
#define ERR_CMDNOTFOUND(nickname, command)                                  (HOSTNAME + " 421 " + nickname + " " + command + " :Unknown command" + CRLF)
#define ERR_NONICKNAMEGIVEN                                                 HOSTNAME + " 431 :No nickname given\r\n" 
#define ERR_ERRONEUSNICKNAME(nickname)                                      (HOSTNAME + " 432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_NICKINUSE(nickname)                                             (HOSTNAME + " 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_USERNOTINCHANNEL(client, nick, channel)                         HOSTNAME + " 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(client, channel)                                   HOSTNAME + " 442 " + client + " " + channel + " " + ": You're not on that channel\r\n"
#define ERR_USERONCHANNEL(client, nick, channel)                            HOSTNAME + " 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n"
#define ERR_NOTREGISTERED(nickname)                                         (HOSTNAME + " 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_NEEDMOREPARAMS(client, command)                                 (HOSTNAME + " 461 " + client + " " + command + " :Not enough parameters." + CRLF)
#define ERR_ALREADYREGISTERED(nickname)                                     (HOSTNAME + " 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_PASSWDMISMATCH(nickname)                                        (HOSTNAME + " 464 " + nickname + " :Password incorrect !" + CRLF )
#define ERR_CHANNELISFULL(nickname, channelname, adress)                    (HOSTNAME + " 471 " + nickname + " #" + channelname +" :Cannot join channel (+l)" +  CRLF)
#define ERR_INVITEONLYCHAN(nickname, channelname, adress)                   (HOSTNAME + " 473 " + nickname + " #" + channelname +" :Cannot join channel (+i)" +  CRLF)
#define ERR_BADCHANNELKEY(nickname, channelname, adress)                    (HOSTNAME + " 475 " +  nickname + " #" + channelname + " :Cannot join channel (+k)" + CRLF )
#define ERR_BADCHANMASK(channelname)                                        (HOSTNAME + " 476 "  + channelname +" :Bad Channel Mask" +  CRLF)
#define ERR_CHANOPRIVSNEEDED(nickname, channelname)                         (HOSTNAME + " 482 " + nickname + " " + channelname + " :You're not channel operator" + CRLF)
#define ERR_CHANOPRIVSNEEDED2(p1, p2)                                       HOSTNAME + " 482 " + p1 + " " + p2 + " :Your privileges are too low\r\n"
#define ERR_INVALIDMODEPARAM(client, channel, mode, modep)                  HOSTNAME + " 696 " + client + " " + channel + " " + mode + " * :Invalid mode k parameter: " + modep + "\r\n"
#define ERR_UNKNOWNMODE(client, modechar)                                   HOSTNAME + " 472 " + client + " " + modechar + " :is unknown mode char to me " + "\r\n"

#endif