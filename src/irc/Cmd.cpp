#include "Cmd.hpp"
Cmd::Cmd(
        string const & cmd,
        vector<string> params,
        User * user
    ) : _cmd(cmd), _params(params), _user(user) { }

Cmd::~Cmd() {}
