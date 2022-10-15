#include "Preset.hpp"
#include "Server.hpp"

void handleStop (int signum) {
    //\b\b to remove ugly ^C
	if (signum == SIGINT || signum == SIGQUIT)
		cout << RED << "\b\bServer stopped. Bye" << DFT << endl;
	exit(0);
}

bool isValid (int ac, char **av, int &port) {
    bool res = false;
    if (ac == 3) {
        port = atoi(av[1]);
        if (port <= 0 || port > 0xffff)
            cerr << RED << "Invalid port range" << DFT << endl;
        else
            res = true;
    } else
        cerr << RED << "Invalid arguments" << DFT << endl;
    return res;
}

int main(int ac, char **av) {
	signal(SIGINT, handleStop);
	signal(SIGQUIT, handleStop);

    int     port;
    string  password = av[2];

    if (!isValid(ac, av, port))   
        exit(1);
    IRC irc(password);
    Server server(port, password, irc);
    server.Init();
    server.Watch();

	return (0);
}