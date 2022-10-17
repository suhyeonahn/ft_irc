#include "Channel.hpp"

Channel::Channel( string const & name, User * creator ) : _name(name) {
    _userList.insert(creator);
    _operList.insert(creator);
}
Channel::~Channel() {

}



bool Channel::IsPrefix( char c ) {
    if (c == CHAN_PREFIX) return true;
    return false;
}
bool Channel::IsValidName(const string &name) {
    if (!IsPrefix(name[0]))  return false;

    static const string InvalidChars(CHAN_INVALID_CHAR);
    const set<char>     nameSet(name.begin() + 1, name.end());
    set<char>::const_iterator it;

    for (it = nameSet.begin(); it != nameSet.end() ; it++)
        if (InvalidChars.find(*it) != string::npos) return false;
    return true;
}
