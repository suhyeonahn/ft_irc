//TODO: IRC class to process cmd sent by client and to response to irc client

#include "IRC.hpp"

IRC::IRC( const string &pw ) : _pw(pw) {}

IRC::~IRC()
{
   for (std::map<int, User *>::iterator it = _userList.begin() ; it != _userList.end() ; ++it )
      delete it->second;

   for (std::map<std::string, Channel *>::iterator it = _channelList.begin() ; 
      it != _channelList.end() ; ++it )
      delete it->second;
}

bool   IRC::ProcessClientMsg( t_ClientMsg const & msg, vector<t_ClientMsg> &res)
{
   User *   user;
   int      fd(msg.first);

   // New user registration
   if (_userList.find(fd) == _userList.end())
      user = (_userList[fd] = new User(fd, _pw));
   else
      user = _userList[fd];
   
   // Split msg to cmd(s)
   std::vector<string>  cmds = ::split(msg.second, SEP_MSG);

   initCmdList();
   // Execute cmd(s)
   for (std::vector<string>::iterator it(cmds.begin()) ; it != cmds.end() ; ++it)
   {
      string cmdStr = setCmd(*it);
      if (!isValid(cmdStr)) // Check if the cmd exists
         ; // TODO: Send an err numeric accordingly
      else if (isImplemented(cmdStr)) // Check if the cmd is implemented in our IRC
      {
         vector<string> params = setParams(*it);
         Cmd cmd = Cmd(cmdStr, params, user, _userList);
         cmd.execute(res);
      }
   }
   return 0;
}
