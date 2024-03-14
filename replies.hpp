#ifndef REPLIES_HPP
#define REPLIES_HPP


#define CRLF "\r\n"


#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
#define ERR_NOSUCHNICK(channelname, name) (": 401 #" + channelname + " " + name + " :No such nick/channel" + CRLF )
#define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_NONICKNAME(nickname) (": 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)
#define ERR_NOTEXTTOSEND(nickname) (": 412 " + nickname + " :No text to send" + CRLF)

#endif