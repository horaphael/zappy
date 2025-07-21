/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** main
*/

#include "Gui.hpp"
#include "Network.hpp"

int disp_usage(void)
{
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
    return 84;
}

int main(int ac, char **av)
{
    int port = -1;
    std::string ip = "127.0.0.1";

    if (ac == 2 && strcmp(av[1], "-help") == 0)
        return disp_usage();
    for (int i = 1; i < ac; i++) {
        if (strcmp(av[i], "-p") == 0 && i + 1 < ac)
            port = std::atoi(av[i + 1]);
        else if (strcmp(av[i], "-h") == 0 && i + 1 < ac)
            ip = av[i + 1];
    }
    if (port == -1)
        return disp_usage();
    Zappy::GUI gui;
    gui.run(port, ip);
    return 0;
}
