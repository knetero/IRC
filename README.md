# IRC Server Implementation in C++

A custom Internet Relay Chat (IRC) server implementation supporting core RFC 1459 protocol specifications, designed for multi-client communication with channel management capabilities.

## Key Features

**Protocol Compliance**  
- Implements core RFC 1459 commands (NICK, USER, JOIN, PRIVMSG)
- Supports standard reply codes (RPL_WELCOME, ERR_NICKNAMEINUSE, etc.)

**Server Functionality**
- Concurrent client handling using I/O multiplexing (select/poll)
- Channel management with operators
- User authentication system
- Channel modes (+n, +t, +i) and topic management

**Client Capabilities**
- **Channel creation/joining** using `JOIN` command:
  - Create new channels by joining non-existent names
  - Join multiple channels simultaneously
  - Handle channel keys/passwords (+k mode)
  - Receive proper error responses (ERR_NOSUCHCHANNEL, ERR_TOOMANYCHANNELS)
- Multi-channel participation
- Private messaging with `PRIVMSG`
