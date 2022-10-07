#include "Preset.hpp"
#include "Server.hpp"

void handleStop (int signum) {
	if (signum == SIGINT || signum == SIGQUIT)
		cout << RED << "\bServer stopped. Bye" << DFT << endl;
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
    string  password;

    if (!isValid(ac, av, port))   
        exit(1);
    Server server(port, string(av[2]));
    server.init();
    server.watch();

	return (0);
}