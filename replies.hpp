#ifndef REPLIES_HPP
#define REPLIES_HPP


#define CRLF "\r\n"
#define ERROR(p1) "ERROR :" + p1 + "\r\n"



#define PRIVMSG(p1, p2, p3, p4, p5) ":" + p1 + "!~" + p2 + "@" + p3 + " PRIVMSG " + p4 + " :" + p5 + "\r\n"
#define NICK(p1, p2, p3, p4) ":" + p1 + "!~" + p2 + "@" + p3 + " NICK :" + p4 + "\r\n"
#define KICK(p1, p2, p3, p4, p5, p6) ":" + p1 + "!~" + p2 + "@" + p3 + " KICK " + p4 + " " + p5 + " :" + p6 + "\r\n"
#define INVITE(p1, p2, p3, p4, p5) ":" + p1 + "!~" + p2 + "@" + p3 + " INVITE " + p4 + " " + p5 + "\r\n"
#define TOPIC(p1, p2, p3, p4, p5) ":" + p1 + "!~" + p2 + "@" + p3 + " TOPIC " + p4 + " :" + p5 + "\r\n"

#define RPL_WELCOME(p1, p2, p3) ": 001 " + p1 + " :Welcome to the Internet Relay Network " + p1 + "!~" + p2 + "@" + p3 + "\r\n"
#define RPL_YOURHOST(p1, p2) ": 002 " + p1 + " :Your host is " + p2 + ", running version 1.0\r\n"
#define RPL_CREATED(p1, p2) ": 003 " + p1 + " :This server has been started " + p2 + "\r\n"

#define RPL_NICKCHANGE(oldnickname, nickname) (oldnickname + " changed his nickname to " + nickname + "."+ CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
#define ERR_NOSUCHNICK(nickname, channelname , username, adress) ":" + nickname + "!" + username + "@" + adress +" 482 " + nickname +" "+ channelname +  " :No such nick/channel" + CRLF
#define ERR_PASSWDMISMATCH(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_NONICKNAMEGIVEN ": 431 :No nickname given\r\n" 
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEUSNICKNAME(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)
#define ERR_NOTEXTTOSEND(nickname) (": 412 " + nickname + " :No text to send" + CRLF)
#define ERR_CHANOPRIVSNEEDED(nickname, channelname) (": 482 " + nickname + " " + channelname + " :You're not channel operator" + CRLF)
#define ERRR_CHANOPRIVSNEEDED(nickname, channelname , username, adress) (":" + nickname + "!" + username + "@" + adress +" 482 " + nickname +" "+ channelname +  " :You're not channel operator" + CRLF)
#define ERR_NOSUCHCHANNEL(nickname, channelname,  adress) ( ":"+adress + " 403 " + nickname + " " + channelname +  " :No such channel" + CRLF)
#define RPL_CHANNELMODEIS(nickname, channelname, modestring) (":"+ nickname + " MODE #" + channelname +  " +" + modestring + CRLF)
#define RPL_SENDMODEIS(nickname, channelname, modestring, sign , arg) ( nickname + " MODE " + channelname +  " : " + sign +  modestring + arg + CRLF)
#define ERR_BADCHANNELKEY(nickname, channelname, adress) ( ":"+adress +" 475 " +  nickname + " #" + channelname + " :Cannot join channel (+k)" + CRLF )
#define ERR_CHANNELISFULL(nickname, channelname, adress) ( ":"+adress +" 471 " + nickname + " #" + channelname +" :Cannot join channel (+l)" +  CRLF)
#define ERR_INVITEONLYCHAN(nickname, channelname, adress) ( ":"+adress +" 473 " + nickname + " #" + channelname +" :Cannot join channel (+i)" +  CRLF)
#define ERR_BADCHANMASK(channelname) ( ": 476 "  + channelname +" :Bad Channel Mask" +  CRLF)
// #define RPL_TOPIC(nickname, channelname, topic) ( ": 332 " + nickname + " " + channelname +" " + topic +  CRLF)
#define RPL_JOIN(nickname, channelname ) ( nickname + " is joining the channel " + channelname  +  CRLF)
#define ERR_NEEDMOREPARAMS(nickname, channelname ) ( ": 332 " + nickname + channelname + " :Not enough parameters" +  CRLF)
#define ERR_NEDMOREPARAMS(nickname, channelname , adress) (  ":"+adress+" 332 " + nickname + channelname + " :Not enough parameters" +  CRLF)
#define RPL_NAMREPLY(adress,nickname, channelname, names ) ( ":"+adress+" 353 " + nickname + " = #" + channelname  +" :"+names+"\r\n")
#define RPL_ENDOFNAMES(adress, nickname, channelname ) ( ":"+adress+" 366 " + nickname  + " #" + channelname + " :End of /NAMES list." +"\r\n")
#define ERR_USERNOTINCHANNEL(p1, p2, p3) ": 441 " + p1 + " " + p2 + " " + p3 + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(p1, p2) ": 442 " + p1 + " " + p2 + " " + ": You're not on that channel\r\n"
#define ERR_USERONCHANNEL(client, nick, channel) ": 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n"
#define ERR_CHANOPRIVSNEEDED2(p1, p2) ": 482 " + p1 + " " + p2 + " :Your privileges are too low\r\n"
#define ERR_CANNOTSENDTOCHAN(p1, p2) " 404 " + p1 + " " + p2 + " :Cannot send to channel\r\n"
#define RPL_INVITING(client, nick, channel) ":341 " + client + " " + nick + " " + channel + "\r\n"
#define RPL_TOPIC(client, channel, topic) ":332 " + client + " " + channel + " " + topic + "\r\n"
#define RPL_TTOPIC(client,username, channel,adress, topic)  ":" + client + "!" + username + "@" + adress + " TOPIC " + channel + " :" + topic +"\r\n" 
#define RPL_NOTOPIC(client, channel) ":331 " + client + " " + channel + " No topic is set\r\n" 
#endif