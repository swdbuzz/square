#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <map>

const unsigned short PORT = 54000;

struct Player {
    sf::Vector2f position;
    sf::Color color;
};

int main() {
    sf::UdpSocket socket;
    socket.setBlocking(false);

    if (socket.bind(PORT) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket\n";
        return 1;
    }

    std::map<sf::IpAddress, Player> players;
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short senderPort;

    std::cout << "Server is running and listening on port " << PORT << "...\n";

    while (true) {

        while (socket.receive(packet, sender, senderPort) == sf::Socket::Done) {
            float x, y;
            if (packet >> x >> y) {
                std::cout << "Received position from " << sender.toString() << ": " << x << ", " << y << "\n";

                players[sender].position = {x, y};
                players[sender].color = sf::Color(255, 0, 0);
            }
        }

        std::cout << "Players currently in the game:\n";
        for (const auto& [ip, player] : players) {
            std::cout << ip.toString() << " -> Position: (" << player.position.x << ", " << player.position.y << ")\n";
        }

        sf::sleep(sf::seconds(0.1f));
    }

    return 0;
}
