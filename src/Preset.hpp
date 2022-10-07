#ifndef PRESET_HPP
# define PRESET_HPP

// c++ standard header
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>

// socket/filedescriptor/system functions  etc
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



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

using namespace std;

#endif