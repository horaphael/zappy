/*
** EPITECH PROJECT, 2025
** parser
** File description:
** parser
*/

#include "ParseServer.hpp"

Zappy::ParseServer::ParseServer(std::shared_ptr<sf::TcpSocket> sock, std::shared_ptr<Zappy::Network> network) : _sock(sock), _network(network)
{
    initCommandMap();
}

void Zappy::ParseServer::initCommandMap()
{
    _commands = {
        {"msz", [this](const auto& args) { handleMsz(args); }},
        {"bct", [this](const auto& args) { handleBct(args); }},
        {"mct", [this](const auto& args) { handleMct(args); }},
        {"tna", [this](const auto& args) { handleTna(args); }},
        {"pnw", [this](const auto& args) { handlePnw(args); }},
        {"ppo", [this](const auto& args) { handlePpo(args); }},
        {"plv", [this](const auto& args) { handlePlv(args); }},
        {"pin", [this](const auto& args) { handlePin(args); }},
        {"pex", [this](const auto& args) { handlePex(args); }},
        {"pbc", [this](const auto& args) { handlePbc(args); }},
        {"pic", [this](const auto& args) { handlePic(args); }},
        {"pie", [this](const auto& args) { handlePie(args); }},
        {"pfk", [this](const auto& args) { handlePfk(args); }},
        {"pdr", [this](const auto& args) { handlePdr(args); }},
        {"pgt", [this](const auto& args) { handlePgt(args); }},
        {"pdi", [this](const auto& args) { handlePdi(args); }},
        {"enw", [this](const auto& args) { handleEnw(args); }},
        {"ebo", [this](const auto& args) { handleEbo(args); }},
        {"edi", [this](const auto& args) { handleEdi(args); }},
        {"sgt", [this](const auto& args) { handleSgt(args); }},
        {"sst", [this](const auto& args) { handleSst(args); }},
        {"seg", [this](const auto& args) { handleSeg(args); }},
        {"smg", [this](const auto& args) { handleSmg(args); }},
        {"suc", [this](const auto& args) { handleSuc(args); }},
        {"sbp", [this](const auto& args) { handleSbp(args); }}
    };
}

std::vector<std::string> Zappy::ParseServer::split(const std::string &line)
{
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

void Zappy::ParseServer::getCommand()
{
    std::array<char, 4096> array;
    std::size_t nbBytes;

    _network->setBlocking(false);
    sf::Socket::Status stat = _network->receiveMessage(array, nbBytes);
    if (stat == sf::Socket::Done) {
        _readBuffer.append(array.data(), nbBytes);
        size_t pos;
        while ((pos = _readBuffer.find('\n')) != std::string::npos) {
            std::string line = _readBuffer.substr(0, pos);
            _readBuffer.erase(0, pos + 1);
            if (line.empty())
                continue;
            std::cout << "Received raw line: '" << line << "'" << std::endl;
            processCommand(line);
        }
    } else if (stat == sf::Socket::Disconnected) {
        std::cerr << "Connection closed" << std::endl;
    } else if (stat == sf::Socket::Error) {
        std::cerr << "Socket error while receiving data" << std::endl;
    }
}

void Zappy::ParseServer::processCommand(const std::string& rawCommand)
{
    std::vector<std::string> tokens = split(rawCommand);

    if (tokens.empty())
        return;
    const std::string& cmd = tokens[0];
    auto it = _commands.find(cmd);
    if (it != _commands.end()) {
        tokens.erase(tokens.begin());
        it->second(tokens);
    } else
        std::cerr << "Unknown command received: " << rawCommand << std::endl;
}

void Zappy::ParseServer::run()
{
    while (_running) {
        getCommand();
        sf::sleep(sf::milliseconds(10));
    }
}

std::string Zappy::ParseServer::getResourceName(int id)
{
    static const std::vector<std::string> resources = {
        "food", "linemate", "deraumere", "sibur",
        "mendiane", "phiras", "thystame"
    };
    return (id >= 0 && id < static_cast<int>(resources.size())) ? resources[id] : "unknown";
}

void Zappy::ParseServer::handleMsz(const std::vector<std::string> &args)
{
    if (args.size() != 2) {
        std::cerr << "Invalid msz arguments" << std::endl;
        return;
    }
    _mapWidth = std::stoi(args[0]);
    _mapHeight = std::stoi(args[1]);
    for (auto& [id, player] : _players)
        player->setMapSize(_mapWidth, _mapHeight);
}

void Zappy::ParseServer::handleBct(const std::vector<std::string> &args)
{
    if (args.size() != 9) {
        std::cerr << "Invalid bct arguments (got " << args.size() << ", expected 9)" << std::endl;
        return;
    }
    try {
        this->_bctNb += 1;
        if (this->_bctNb >= this->getMapHeight() * this->getMapWidth())
            this->_mapReady = true;
        int x = std::stoi(args[0]);
        int y = std::stoi(args[1]);
        auto resources = std::map<std::string, int> {
            {"food", std::stoi(args[2])},
            {"linemate", std::stoi(args[3])},
            {"deraumere", std::stoi(args[4])},
            {"sibur", std::stoi(args[5])},
            {"mendiane", std::stoi(args[6])},
            {"phiras", std::stoi(args[7])},
            {"thystame", std::stoi(args[8])}
        };
        std::pair<int, int> coords(y, x);
        _tiles[coords].setResources(resources);
    } catch (const std::exception& e) {
        std::cerr << "Error processing bct command: " << e.what() << std::endl;
    }
}

void Zappy::ParseServer::handleMct(const std::vector<std::string>& args)
{
    if (args.size() % 9 != 0) {
        std::cerr << "Invalid mct arguments (count: " << args.size() << ")" << std::endl;
        return;
    }
    for (size_t i = 0; i < args.size(); i += 9) {
        std::vector<std::string> tileArgs(args.begin() + i, args.begin() + i + 9);
        handleBct(tileArgs);
    }
    std::cout << "Processed mct command with " << args.size() / 9 << " tiles" << std::endl;
}

void Zappy::ParseServer::handlePin(const std::vector<std::string>& args)
{
    if (args.size() != 10) {
        std::cerr << "Invalid pin arguments" << std::endl;
        return;
    }
    int playerId = std::stoi(args[0].substr(1));
    if (_players.count(playerId)) {
        auto& player = _players[playerId];
        player->setInventory({
            {"food", std::stoi(args[3])},
            {"linemate", std::stoi(args[4])},
            {"deraumere", std::stoi(args[5])},
            {"sibur", std::stoi(args[6])},
            {"mendiane", std::stoi(args[7])},
            {"phiras", std::stoi(args[8])},
            {"thystame", std::stoi(args[9])}
        });
        std::cout << "Player #" << playerId << " inventory updated" << std::endl;
    }
}

void Zappy::ParseServer::handleTna(const std::vector<std::string>& args)
{
    if (args.empty()) {
        std::cerr << "Invalid tna command: missing team name" << std::endl;
        return;
    }
    std::string teamName = args[0];
    if (std::find(_teamNames.begin(), _teamNames.end(), teamName) == _teamNames.end()) {
        _teamNames.push_back(teamName);
        std::cout << "Registered new team: " << teamName << std::endl;
    }
}

void Zappy::ParseServer::handlePnw(const std::vector<std::string>& args)
{
    if (args.size() != 6) {
        std::cerr << "Invalid pnw command" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        int playerX = std::stoi(args[1]);
        int playerY = std::stoi(args[2]);
        int orientation = std::stoi(args[3]);
        int level = std::stoi(args[4]);
        std::string teamName = args[5];
        auto player = std::make_shared<Player>(playerId, playerX, playerY, static_cast<Direction>(orientation), teamName, _mapWidth, _mapHeight);
        player->setLevel(level);
        _players[playerId] = player;
    } catch (...) {
        std::cerr << "Error processing pnw command" << std::endl;
    }
}

void Zappy::ParseServer::handlePpo(const std::vector<std::string>& args)
{
    if (args.size() != 1 && args.size() != 4) {
        std::cerr << "Invalid ppo command: expected 1 or 4 args" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        if (_players.count(playerId)) {
            auto& player = _players[playerId];
            if (args.size() == 4) {
                int x = std::stoi(args[1]);
                int y = std::stoi(args[2]);
                int orientation = std::stoi(args[3]);
                player->setPosition(x, y);
                player->setDirection(static_cast<Direction>(orientation));
            }
            std::cout << "Player #" << playerId << " position: (" << player->getX() << "," << player->getY() << ")" << std::endl;
            player->updateSpriteTexture();
        }
    } catch (...) {
        std::cerr << "Error processing ppo command" << std::endl;
    }
}

void Zappy::ParseServer::handlePlv(const std::vector<std::string>& args)
{
    if (args.size() != 2) {
        std::cerr << "Invalid plv command: expected 2 args" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        int level = std::stoi(args[1]);
        if (_players.count(playerId)) {
            _players[playerId]->setLevel(level);
            std::cout << "Player #" << playerId << " is now level " << level << std::endl;
        }
    } catch (...) {
        std::cerr << "Error processing plv command" << std::endl;
    }
}

void Zappy::ParseServer::handlePex(const std::vector<std::string>& args)
{
    if (args.size() != 1) {
        std::cerr << "Invalid pex command: expected 1 arg" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        std::cout << "Player #" << playerId << " was expelled" << std::endl;
    } catch (...) {
        std::cerr << "Error processing pex command" << std::endl;
    }
}

void Zappy::ParseServer::handlePbc(const std::vector<std::string>& args)
{
    if (args.size() < 2) {
        std::cerr << "Invalid pbc command: expected at least 2 args" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        std::string message = args[1];
        for (size_t i = 2; i < args.size(); i++)
            message += " " + args[i];
        auto player = _players[playerId];
        if (player->getWindow())
            player->drawBroadcast(*player->getWindow(), player->getX(), player->getY(), player->getTileSize());
        std::cout << "Broadcast from player #" << playerId << ": " << message << std::endl;
    } catch (...) {
        std::cerr << "Error processing pbc command" << std::endl;
    }
}

void Zappy::ParseServer::handlePic(const std::vector<std::string>& args)
{
    if (args.size() < 4) {
        std::cerr << "Invalid pic command: expected at least 4 args" << std::endl;
        return;
    }
    try {
        int x = std::stoi(args[0]);
        int y = std::stoi(args[1]);
        int level = std::stoi(args[2]);
        std::vector<int> players;
        for (size_t i = 3; i < args.size(); i++)
            players.push_back(std::stoi(args[i].substr(1)));
        std::cout << "Incantation started at (" << x << "," << y << ") level " << level << " with players: ";
        for (int id : players)
            std::cout << "#" << id << " ";
        std::cout << std::endl;
        _tiles[{x, y}].setIncantation(true, level);
    } catch (...) {
        std::cerr << "Error processing pic command" << std::endl;
    }
}

void Zappy::ParseServer::handlePie(const std::vector<std::string>& args)
{
    if (args.size() != 3) {
        std::cerr << "Invalid pie command: expected 3 args (X Y R)" << std::endl;
        return;
    }
    try {
        int x = std::stoi(args[0]);
        int y = std::stoi(args[1]);
        int result = std::stoi(args[2]);
        std::cout << "Incantation at (" << x << "," << y << ") " << (result ? "succeeded" : "failed") << std::endl;
    } catch (...) {
        std::cerr << "Error processing pie command" << std::endl;
    }
}

void Zappy::ParseServer::handlePfk(const std::vector<std::string>& args)
{
    if (args.size() != 1) {
        std::cerr << "Invalid pfk command: expected 1 arg (#n)" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        if (_players.count(playerId))
            std::cout << "Player #" << playerId << " laid an egg" << std::endl;
    } catch (...) {
        std::cerr << "Error processing pfk command" << std::endl;
    }
}

void Zappy::ParseServer::handlePdr(const std::vector<std::string>& args)
{
    if (args.size() != 2) {
        std::cerr << "Invalid pdr command: expected 2 args (#n i)" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        int resourceId = std::stoi(args[1]);
        if (_players.count(playerId)) {
            std::string resourceName = getResourceName(resourceId);
            std::cout << "Player #" << playerId << " dropped " << resourceName << std::endl;
        }
    } catch (...) {
        std::cerr << "Error processing pdr command" << std::endl;
    }
}

void Zappy::ParseServer::handlePgt(const std::vector<std::string>& args)
{
    if (args.size() != 2) {
        std::cerr << "Invalid pgt command: expected 2 args (#n i)" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        int resourceId = std::stoi(args[1]);
        if (_players.count(playerId)) {
            std::string resourceName = getResourceName(resourceId);
            std::cout << "Player #" << playerId << " collected " << resourceName << std::endl;
        }
    } catch (...) {
        std::cerr << "Error processing pgt command" << std::endl;
    }
}

void Zappy::ParseServer::handlePdi(const std::vector<std::string>& args)
{
    if (args.size() != 1) {
        std::cerr << "Invalid pdi command: expected player ID" << std::endl;
        return;
    }
    try {
        int playerId = std::stoi(args[0].substr(1));
        _players.erase(playerId);
        std::cout << "Player #" << playerId << " has been removed" << std::endl;
    } catch (...) {
        std::cerr << "Error processing pdi command" << std::endl;
    }
}

void Zappy::ParseServer::handleEnw(const std::vector<std::string>& args)
{
    if (args.size() != 4) {
        std::cerr << "Invalid Enw command" << std::endl;
        return;
    }
    try {
        int eggId = std::stoi(args[0].substr(1));
        int playerId = std::stoi(args[1].substr(1));
        int x = std::stoi(args[2]);
        int y = std::stoi(args[3]);
        auto egg = std::make_shared<Egg>(eggId, playerId, x, y);
        _eggs[eggId] = egg;
        std::cout << "Egg #" << eggId << " has been laid by player #" << playerId << " at position (" << x << "," << y << ")" << std::endl;
    } catch (...) {
        std::cerr << "Error processing enw command" << std::endl;
    }
}

void Zappy::ParseServer::handleEbo(const std::vector<std::string>& args)
{
    if (args.size() != 1) {
        std::cerr << "Invalid Ebo command" << std::endl;
        return;
    }
    try {
        int eggId = std::stoi(args[0].substr(1));
        if (_eggs.find(eggId) == _eggs.end()) {
            std::cerr << "Unknown egg ID: " << eggId << std::endl;
            return;
        }
        _eggs[eggId]->setHatched(true);
        std::cout << "Egg #" << eggId << " has hatched" << std::endl;
    } catch (...) {
        std::cerr << "Error processing ebo command" << std::endl;
    }
}

void Zappy::ParseServer::handleEdi(const std::vector<std::string>& args)
{
    if (args.size() != 1) {
        std::cerr << "Invalid Edi command" << std::endl;
        return;
    }
    try {
        int eggId = std::stoi(args[0].substr(1));
        if (_eggs.find(eggId) == _eggs.end()) {
            std::cerr << "Unknown egg ID: " << eggId << std::endl;
            return;
        }
        _eggs.erase(eggId);
        std::cout << "Egg #" << eggId << " has died" << std::endl;
    } catch (...) {
        std::cerr << "Error processing edi command" << std::endl;
    }
}

void Zappy::ParseServer::handleSgt(const std::vector<std::string>& args)
{
    if (args.size() != 1 && args.size() != 0) {
        std::cerr << "Invalid sgt command: expected 0 or 1 arg" << std::endl;
        return;
    }
    try {
        if (args.size() == 1) {
            _timeUnit = std::stoi(args[0]);
            std::cout << "Time unit set to: " << _timeUnit << std::endl;
        } else {
            std::cout << "Current time unit: " << _timeUnit << std::endl;
        }
    } catch (...) {
        std::cerr << "Error processing sgt command" << std::endl;
    }
}

void Zappy::ParseServer::handleSst(const std::vector<std::string>& args)
{
    if (args.size() != 1) {
        std::cerr << "Invalid sst command: expected time unit" << std::endl;
        return;
    }
    try {
        _timeUnit = std::stoi(args[0]);
        std::cout << "Time unit modified to: " << _timeUnit << std::endl;
    } catch (...) {
        std::cerr << "Error processing sst command" << std::endl;
    }
}

void Zappy::ParseServer::handleSeg(const std::vector<std::string>& args)
{
    if (args.size() != 1) {
        std::cerr << "Invalid seg command: expected winning team name" << std::endl;
        return;
    }
    try {
        std::string winningTeam = args[0];
        _gameOver = true;
        _winningTeam = winningTeam;
        std::cout << "Game over! Winning team: " << winningTeam << std::endl;
    } catch (...) {
        std::cerr << "Error processing seg command" << std::endl;
    }
}

void Zappy::ParseServer::handleSmg(const std::vector<std::string>& args)
{
    if (args.empty()) {
        std::cerr << "Invalid smg command: expected message" << std::endl;
        return;
    }
    try {
        std::string message;
        for (const auto& arg : args)
            message += arg + " ";
        std::cout << "Server message: " << message << std::endl;
    } catch (...) {
        std::cerr << "Error processing smg command" << std::endl;
    }
}

void Zappy::ParseServer::handleSuc(const std::vector<std::string>& args)
{
    static_cast<void>(args);
    std::cerr << "Server responded: unknown command" << std::endl;
}

void Zappy::ParseServer::handleSbp(const std::vector<std::string>& args)
{
    static_cast<void>(args);
    std::cerr << "Server responded: bad parameter" << std::endl;
}
