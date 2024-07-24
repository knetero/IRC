#ifndef REPLIES_HPP
#define REPLIES_HPP


#define CRLF "\r\n"
#define ERROR(p1) "ERROR :" + p1 + "\r\n"

#define PRIVMSG(p1, p2, p3, p4, p5) ":" + p1 + "!~" + p2 + "@" + p3 + " PRIVMSG " + p4 + " :" + p5 + "\r\n"
#define NICK(p1, p2, p3, p4) ":" + p1 + "!~" + p2 + "@" + p3 + " NICK :" + p4 + "\r\n"

#define RPL_NICKCHANGE(oldnickname, nickname) (oldnickname + " changed his nickname to " + nickname + "."+ CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
#define ERR_NOSUCHNICK(channelname, name) (": 401 #" + channelname + " " + name + " :No such nick/channel" + CRLF )
#define ERR_PASSWDMISMATCH(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_NONICKNAMEGIVEN ": 431 :No nickname given\r\n" 
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEUSNICKNAME(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)
#define ERR_NOTEXTTOSEND(nickname) (": 412 " + nickname + " :No text to send" + CRLF)
#define ERR_CHANOPRIVSNEEDED(nickname, channelname) (": 482 " + nickname + " " + channelname + " :You're not channel operator" + CRLF)
#define ERR_NOSUCHCHANNEL(nickname, channelname) (": 403 " + nickname + " " + channelname +  " :No such channel" + CRLF)
#define RPL_CHANNELMODEIS(nickname, channelname, modestring) (": 324 " + nickname + " " + channelname +  " : " + modestring + CRLF)
#define RPL_SENDMODEIS(nickname, channelname, modestring, sign , arg) ( nickname + " MODE " + channelname +  " : " + sign +  modestring + arg + CRLF)
#define ERR_BADCHANNELKEY(nickname, channelname) ( ": 475 " +  nickname + " " + channelname + " :Cannot join channel (+k)" + CRLF )
#define ERR_CHANNELISFULL(nickname, channelname) ( ": 471 " + nickname + " " + channelname +" :Cannot join channel (+l)" +  CRLF)
#define ERR_INVITEONLYCHAN(nickname, channelname) ( ": 473 " + nickname + " " + channelname +" :Cannot join channel (+i)" +  CRLF)
#define ERR_BADCHANMASK(channelname) ( ": 476 "  + channelname +" :Bad Channel Mask" +  CRLF)
#define RPL_TOPIC(nickname, channelname, topic) ( ": 332 " + nickname + " " + channelname +" " + topic +  CRLF)
#define RPL_JOIN(nickname, channelname ) ( nickname + " is joining the channel " + channelname  +  CRLF)
#define ERR_NEEDMOREPARAMS(nickname, channelname ) ( ": 332 " + nickname + channelname + " :Not enough parameters" +  CRLF)
#define RPL_NAMREPLY(nickname, channelname, names ) ( ": 353  :127.0.0.1 " + nickname + " = #" + channelname + " : " +names+ CRLF)
#define RPL_ENDOFNAMES(nickname, channelname ) ( ": 366 :127.0.0.1 " + nickname + " #" + channelname + " :End of /NAMES list" + CRLF)
#endif