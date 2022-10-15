#include "Channel.hpp"

Channel::Channel( string const & name, User * creator ) : _name(name) {
    _userList.insert(creator);
    _operList.insert(creator);
}
Channel::~Channel() {

}