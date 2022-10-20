//TODO: IRC class to process cmd sent by client and to response to irc client

#include "IRC.hpp"

IRC::IRC( const string &pw ) : _pw(pw) {}

IRC::~IRC()
{
   for (map<int, User *>::iterator it = _userList.begin() ; it != _userList.end() ; ++it )
      delete it->second;

   for (map<string, Channel *>::iterator it = _chanList.begin() ; 
      it != _chanList.end() ; ++it )
      delete it->second;
}

bool   IRC::ProcessClientMsg( t_ClientMsg const & msg, vector<t_ClientMsg> &res)
{
   User *   user;
   int      fd(msg.first);
   string   cmdStr = "";

   // New user registration
   if (_userList.find(fd) == _userList.end()) 
      user = (_userList[fd] = new User(fd, _pw));
   else
      user = _userList[fd];
   
   // Split msg to cmd(s)
   vector<string>  cmds = ::split(msg.second, SEP_MSG);

   initCmdList();

   // Execute cmd(s)
   for (std::vector<string>::iterator it(cmds.begin()) ; it != cmds.end() ; ++it)
   {
      cmdStr = setCmd(*it);

      // Check if the cmd exists
      // TODO: Send an err numeric accordingly 
      if (!isValid(cmdStr)) 
         Cmd::PushToRes(fd, getServReply(user, ERR_UNKNOWNCOMMAND, (string[]){cmdStr}), res);
      else if (isImplemented(cmdStr)) // Check if the cmd is implemented in our IRC
      {
         vector<string> params = setParams(*it);
         Cmd cmd(cmdStr, params, user, _userList, _chanList);
         cmd.execute(res);
      }
   }
   return 0;
}

void  IRC::test()
{
            bool plus;
            //  parse modestring
            string params = "i0+23l--on++=o-";
            vector<string> modeStr = splitModeStr(params, "+-");
            for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
            {
               string tmp = *it;
               if (tmp[0] == '+')
                  plus = true;
               else if (tmp[1] == '-')
                  plus = false;
               else
                  ; //  skip err
               for(string::size_type i = 1; i < tmp.size(); ++i)
                  std::cout << tmp[i] << endl;
            }
}

void IRC::DEBUG() {
   //userList
   if (_userList.size()) {
      map<int , User *>::iterator userIt;
      cout << "- User List: " << endl;
      for (userIt = _userList.begin(); userIt != _userList.end(); ++userIt) {
         //[fd] nick 
         cout << "\t- [" << userIt->first << "] " << userIt->second->getNick() << endl;
      }
      cout << endl;
   }

   //chanlist
   if (_chanList.size()) {
      map<string , Channel *>::iterator chanIt;
      cout << "- Channel List: " << endl;
      for (chanIt = _chanList.begin(); chanIt != _chanList.end(); ++chanIt) {
         Channel *chan = chanIt->second;
         //Channel name
         cout << "\t[ " << chanIt->first << " ]" << endl;
         if (!chan->_topic.empty())
            cout << "\t - topic: " << chan->_topic << endl;
         if (chan->_userList.size()) {
            cout << "\t - users: [ ";
            set<User *>::iterator userIt;
            for (userIt = chan->_userList.begin(); userIt != chan->_userList.end(); ++userIt) {
               cout << "(" << (*userIt)->_fd << ")" << (*userIt)->_nick << ", ";
            }
            cout << "]" << endl;
         }
      }
      cout << endl;
   }
}