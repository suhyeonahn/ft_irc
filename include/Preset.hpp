#ifndef PRESET_HPP
# define PRESET_HPP

// c++ standard header
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iterator>
#include <set>
#include <algorithm>
#include <sstream>
#include <string>

// socket/filedescriptor/system functions  etc
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <errno.h>


// Color to put on stdout
# ifndef COLOR
#  define RED "\033[0;31m"
#  define GRN "\033[0;32m"
#  define YEL "\033[0;33m"
#  define BLU "\033[0;34m"
#  define MAG "\033[0;35m" //purple
#  define CYN "\033[0;36m" //emerald
#  define WHT "\033[0;37m"
#  define DFT "\033[0m"
# endif

# define USR_HOST "user.irc-suky.42paris.fr" // user's host
# define SERV_HOST "irc-suky.42paris.fr" // server's host

# define SEP_MSG "\r\n"
# define SEP_LEN 2

// OPER config
# define OPER_NAME "suky"
# define OPER_PASS "1234"

using namespace std;

// Declaration classes to use in irc
class User;
class Server;
class Client;
class Channel;
class Cmd;

typedef std::pair<int, string>	t_ClientMsg;

#endif