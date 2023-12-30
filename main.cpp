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

    int width = 11;
    //std::cout << "Enter width: ";
    //std::cin >> width;
    int height = 11;
    //std::cout << "Enter height: ";
    //std::cin >> height;

    auto *world = new World(width, height);
    auto *ant = new Ant(5, 5, world->getBlock(5, 5));
    std::cout << ant->toString() << std::endl;
    std::cout << ant->getCurrentBlock()->toString() << std::endl;


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
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                window.draw(world->getRectMap(blockSize)[i][j]);
            }
        }
        window.draw(ant->getSprite());
        window.display();
        usleep(300000);
        ant->move();
        std::cout << ant->toString() << std::endl;
        ant->setCurrentBlock(world->getBlock(ant->getX(), ant->getY()));

        std::cout << "block:" << ant->getCurrentBlock()->toString() << std::endl;
        std::cout << "mravec:" << ant->getX() << " " << ant->getY() << std::endl;
    }
    //mySocket->sendData("qqqqq");
    return 0;
}