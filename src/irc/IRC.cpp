//TODO: IRC class to process cmd sent by client and to response to irc client

 #include "IRC.hpp"

IRC::IRC(){}

std::vector<std::string> split( std::string const & str, std::string const delim )
{
    std::vector<std::string> tokens;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        tokens.push_back(str.substr(start, end - start));
    }
    return tokens;
}

bool   IRC::ProcessClientMsg( t_ClientMsg const & msg )
{
   User  *user;
   int   fd(msg.first);

   // New user registration
   if (_userList.find(fd) == _userList.end())
      user = (_userList[fd] = new User(fd));
   else
      user = _userList[fd];
   
   // Split msg to cmd(s)
   std::vector<string>  cmds = ::split(msg.second, SEP_MSG);

   // Execute cmd(s)
   for (std::vector<string>::iterator it(cmds.begin()) ; it != cmds.end() ; ++it)
   {
      Cmd cmd(user, *it);
      if (!cmd.isValid()) // Check if the cmd exists
         ; // TODO: Send an err numeric accordingly
      else if (cmd.isImplemented()) // Check if the cmd is implemented in our IRC
         cmd.execute();
   }
   return 0;
}