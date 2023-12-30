#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "my_socket.h"
#include "World.h"
#include "Ant.h"


int main() {
    //MySocket *mySocket = MySocket::createConnection("frios2.fri.uniza.sk", 11333);
    //mySocket->sendData("Hello world");

    auto *world = new World(800, 600, 10, 10);
    Ant *ant = new Ant(5, 5, world->getBlock(5, 5));
    std::cout << to_string(world->getBlock(0, 0)->getBlockType()) << std::endl;
    world->setBlock(0, 0, BLACK);
    world->setBlock(1, 0, BLACK);
    world->setBlock(2, 0, WHITE);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Langston's Ant");
    sf::Texture texture;
    if (!texture.loadFromFile("images/ant.png")) {
        std::cout << "Could not load enemy texture" << std::endl;
        return 0;
    }
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
/*
        window.clear(sf::Color::Black);

        sf::Sprite image;
        image.setTexture(texture);
        */
        window.clear();
        world->printWorld(window, ant);
        window.display();

    }
    //mySocket->sendData("qqqqq");
    return 0;
}