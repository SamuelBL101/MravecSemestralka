#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "my_socket.h"
#include "World.h"
#include "Ant.h"


int main() {
    const float blockSize = 70.f;
    //MySocket *mySocket = MySocket::createConnection("frios2.fri.uniza.sk", 11333);
    //mySocket->sendData("Hello world");

    auto *world = new World(10, 10);
    auto *ant = new Ant(5, 5, world->getBlock(5, 5));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Draw Map");


    float scale = static_cast<float>(blockSize) / ant->getSprite().getTexture()->getSize().x;
    ant->scale(scale, scale);
    ant->goTo(ant->getX() * blockSize, ant->getY() * blockSize);

    if (window.isOpen()) {
    }
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                window.draw(world->getRectMap(blockSize)[i][j]);
            }
        }
        window.draw(ant->getSprite());
        window.display();
    }
    //mySocket->sendData("qqqqq");
    return 0;
}