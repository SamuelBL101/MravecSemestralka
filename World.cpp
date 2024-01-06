//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <fstream>
#include <SFML/Graphics/Sprite.hpp>
#include <thread>
#include <cmath>
#include <SFML/Window/Mouse.hpp>
#include "World.h"
#include "Button.h"
#include "my_socket.h"

World::World(int width, int height, int numberOfAnts, bool random, float size) {
    this->width = width;
    this->height = height;
    this->sizeOfBlock = size;

    this->ants = std::vector<Ant>();
    if (random) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }
    this->map = std::vector<std::vector<Block>>(height, std::vector<Block>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            if (random) {
                map[i][j] = Block(static_cast<BlockType>(std::rand() % 2));
            } else {
                map[i][j] = Block(WHITE);
            }
            map[i][j].setXandY(i, j);
        }
    }

    float scale = static_cast<float>(size) / 960;
    //Ant a = Ant(this->getBlock(width / 2, height/2), UP, true, this->sizeOfBlock);
    for (int i = 0; i < numberOfAnts; ++i) {
        Ant a = Ant(this->getBlock(std::rand() % width, std::rand() % height), UP, true, this->sizeOfBlock);
        a.setColor(A_BLUE);
        a.scale(scale, scale);
        a.goTo(a.getX() * size, a.getY() * size);
        ants.push_back(a);
    }
}


Block *World::getBlock(int x, int y) {
    return &map[y][x];
}

void World::setBlock(int x, int y, BlockType blockType) {
    this->map[y][x].setBlockType(blockType);
}

int World::getWidth() const {
    return this->width;
}

int World::getHeight() const {
    return this->height;
}

std::vector<std::vector<sf::RectangleShape>> World::getRectMap(float size) {
    std::vector<std::vector<sf::RectangleShape>> rectMap(height, std::vector<sf::RectangleShape>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            rectMap[i][j].setSize(sf::Vector2f(size, size));
            rectMap[i][j].setPosition(j * size, i * size);
            rectMap[i][j].setFillColor(map[i][j].getBlockType() == BLACK ? sf::Color::Black : sf::Color::White);
            rectMap[i][j].setOutlineColor(sf::Color::Blue);
            rectMap[i][j].setOutlineThickness(2);
        }
    }

    return rectMap;
}

void World::changeBlockType(int x, int y) {
    this->map[y][x].setBlockType(this->map[y][x].getBlockType() == BLACK ? WHITE : BLACK);
}

void World::drawMap(sf::RenderWindow *window) {
    sf::VertexArray vertexArray = this->getVertexArray(this->sizeOfBlock);
    window->draw(vertexArray);

    this->collisionDetection();

    for (int i = 0; i < this->ants.size(); ++i) {
        if (ants.at(i).getX() < 0 || ants.at(i).getX() >= this->width || ants.at(i).getY() < 0 ||
            ants.at(i).getY() >= this->height) {
            std::cout << "out of bounds" << std::endl;
            std::swap(ants.at(i), ants.at(ants.size() - 1));
            ants.pop_back();
        }

    }
//    for (int i = 0; i < this->ants.size(); ++i) {
//        window->draw(this->ants.at(i).getSprite());
//    }

}

void World::addAnt(Ant ant) {
    this->ants.push_back(ant);
}

void World::move() {
    for (auto &ant: ants) {
        ant.move();
        ant.setCurrentBlock(this->getBlock(ant.getX(), ant.getY()));
    }

}

void World::saveToFile(std::string fileName) {
    std::ofstream file;
    file.open(fileName);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << fileName << std::endl;
        return;
    } else {
        std::cout << "File opened successfully" << std::endl;
    }
    file << this->width << " " << this->height << std::endl;
    /*
    for (auto &ant: *this->ants) {
        file << ant->toString() << std::endl;
    }
     */
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            file << this->map[i][j].getBlockType() << " ";
        }
        file << std::endl;
    }
    file.close();

}

void World::loadFromFile(std::string &fileName, int lower) {
    std::ifstream file;
    file.open(fileName);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << fileName << std::endl;
        return;
    } else {
        std::cout << "File opened successfully" << std::endl;
    }
    file >> this->width >> this->height;
    this->sizeOfBlock = lower / (width > height ? width : height);

    this->map = std::vector<std::vector<Block>>(height, std::vector<Block>(width));

    this->ants = std::vector<Ant>();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            int blockType;
            file >> blockType;
            map[i][j] = Block((BlockType) blockType);
            map[i][j].setXandY(i, j);
        }
    }
    file.close();
}

int World::getNumberOfAnts() {
    return this->ants.size();
}

World::World(std::string fileName, int numberOfAnts, float size) {
    this->sizeOfBlock = size;
    this->loadFromFile(fileName, size);
    float scale = static_cast<float>(this->sizeOfBlock) / 960;
    for (int i = 0; i < numberOfAnts; ++i) {
        Ant a = Ant(this->getBlock(rand() % 10, rand() % 10), UP, true, this->sizeOfBlock);
        a.setColor(A_BLACK);
        a.scale(scale, scale);
        a.goTo(a.getX() * this->sizeOfBlock, a.getY() * this->sizeOfBlock);
        ants.push_back(a);
    }
}

void World::setAntsLogic(int logic) {
    this->logicOfAnts = logic;
}

void World::setAntColor(ColoredAnt color, int antIndex) {
    this->ants.at(antIndex).setColor(color);
}

void World::collisionDetection() {
    for (int i = 0; i < this->ants.size(); ++i) {
        for (int j = i + 1; j < this->ants.size(); ++j) {
            if (this->ants.at(i).getCurrentBlock() == this->ants.at(j).getCurrentBlock() &&
                this->ants.at(i).getColor() !=
                this->ants.at(j).getColor()) {
                std::cout << this->ants.size() << " collision ";
                switch (logicOfAnts) {
                    case 0:
                        ants.erase(ants.begin() + i);
                        break;
                    case 1:
                        ants.erase(ants.begin() + i);
                        ants.erase(ants.begin() + j);
                        break;
                    case 2:
                        ants.at(i).changeBehavior();
                        break;
                }
                std::cout << this->ants.size() << std::endl;
            }
        }
    }
}

bool World::isPaused() const {
    return paused;
}

void World::setPaused(bool paused) {
    World::paused = paused;
}

void World::setSizeOfBlock(float size) {
    this->sizeOfBlock = size;
}

sf::VertexArray World::getVertexArray(float size) {
    sf::VertexArray vertexArray(sf::Quads, 4 * height * width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Calculate the position of the current quad
            float x = j * size;
            float y = i * size;

            // Set the vertices of the quad
            vertexArray[4 * (i * width + j)].position = sf::Vector2f(x, y);
            vertexArray[4 * (i * width + j) + 1].position = sf::Vector2f(x + size, y);
            vertexArray[4 * (i * width + j) + 2].position = sf::Vector2f(x + size, y + size);
            vertexArray[4 * (i * width + j) + 3].position = sf::Vector2f(x, y + size);

            // Set the color based on the block type
            sf::Color fillColor = (map[i][j].getBlockType() == BLACK) ? sf::Color::Black : sf::Color::White;
            for (int k = 0; k < 4; ++k) {
                vertexArray[4 * (i * width + j) + k].color = fillColor;
            }
        }
    }

    return vertexArray;
}

float World::getSizeOfBlock() const {
    return this->sizeOfBlock;
}

void World::setBlockSize(int size) {
    this->sizeOfBlock = size;
}

void World::changeAntBehaviour() {
    for (auto &ant: this->ants) {
        ant.changeBehavior();
    }
}

void World::changeBehaviourOfAnts() {
    for (auto &ant: this->ants) {
        ant.changeBehavior();
    }
}

void World::threadAntMovement() {
    while (this->getNumberOfAnts() > 0) {
        std::unique_lock<std::mutex> globalLock(this->worldMutex);
        while (this->isPaused()) {
            this->worldCv.wait(globalLock);
        }
        this->move();
        globalLock.unlock();
        this->worldCv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

void World::threadDisplay() {
    sf::RenderWindow window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width, 0.9 * sf::VideoMode::getDesktopMode().height),
            "SFML Fullscreen Windowed");
    window.setPosition(sf::Vector2i(0, 0));
    Button uploadMapButton(20, sf::VideoMode::getDesktopMode().height * 0.85, 100, 50, "Upload Map");

    while (window.isOpen() && this->getNumberOfAnts() > 0) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (uploadMapButton.handleEvent(event)) {
                std::string filename = "mapz.txt";
                uploadMap(filename, 18888);
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.x >= 0 && event.mouseButton.x <= this->getWidth() * this->getSizeOfBlock() &&
                        event.mouseButton.y >= 0 &&
                        event.mouseButton.y <= this->height * this->sizeOfBlock) {
                        int blockClickedX = std::floor(event.mouseButton.x / this->sizeOfBlock);
                        int blockClickedY = std::floor(event.mouseButton.y / this->sizeOfBlock);
                        this->changeBlockType(blockClickedX, blockClickedY);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 112) {
                        this->setPaused(!this->isPaused());
                        this->worldCv.notify_one();
                    }
                    if (event.text.unicode == 105) {
                        this->changeAntBehaviour();
                    }

                }
            }

            if (event.type == sf::Event::Closed) {
                window.close();
                this->worldCv.notify_one();
                if (this->getNumberOfAnts() <= 0)
                    break;
                else {
                    this->saveToFile("mapz.txt");
                    std::exit(0);
                }
            }
        }
        {
            std::unique_lock<std::mutex> locker(this->worldMutex);
            window.clear();
            this->drawMap(&window);
            uploadMapButton.draw(window);
            window.display();
        }
    }
}

void World::uploadMap(std::string &mapName, short port) {

    this->saveToFile(mapName);
    MySocket *mySocket = MySocket::createConnection("frios2.fri.uniza.sk", port);
    std::string message = mapName + ";" + convertFileToString(mapName);
    mySocket->sendData(message);
    delete mySocket;
    mySocket = nullptr;

}

std::string World::convertFileToString(const std::string &filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            buffer << line << ";";
        }
        file.close();
    }
    return buffer.str();

}

std::vector<Ant> World::getAnts() {
    return this->ants;
}


//std::condition_variable World::getWorldCv() {
//    return worldCv;
//}
//void antMovement(World &world) {
//    sf::RenderWindow window(
//            sf::VideoMode(sf::VideoMode::getDesktopMode().width, 0.9 * sf::VideoMode::getDesktopMode().height),
//            "SFML Fullscreen Windowed");
//    window.setPosition(sf::Vector2i(0, 0));
//    Button uploadMapButton(20, sf::VideoMode::getDesktopMode().height * 0.85, 100, 50, "Upload Map");
//
//    while (window.isOpen() && world.getNumberOfAnts() > 0) {
//
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (uploadMapButton.handleEvent(event)){
//                std::string filename = "mapz.txt";
//                uploadMap(world, filename, 18888);
//            }
//            if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    if (event.mouseButton.x >= 0 && event.mouseButton.x <= world.getWidth() * world.getSizeOfBlock() &&
//                        event.mouseButton.y >= 0 &&
//                        event.mouseButton.y <= world.getHeight() * world.getSizeOfBlock()) {
//                        int blockClickedX = std::floor(event.mouseButton.x / world.getSizeOfBlock());
//                        int blockClickedY = std::floor(event.mouseButton.y / world.getSizeOfBlock());
//                        world.changeBlockType(blockClickedX, blockClickedY);
//                    }
//                }
//            }
//            if (event.type == sf::Event::TextEntered) {
//                if (event.text.unicode < 128) {
//                    if (event.text.unicode == 112) {
//                        world.setPaused(!world.isPaused());
//                        world.getWorldCv().notify_one();
//                    }
//                    if (event.text.unicode == 105) {
//                        world.changeAntBehaviour();
//                    }
//
//                }
//            }
//
//            if (event.type == sf::Event::Closed) {
//                window.close();
//                world.getWorldCv().notify_one();
//                if (world.getNumberOfAnts() <= 0)
//                    break;
//                else {
//                    world.saveToFile("mapz.txt");
//                    std::exit(0);
//                }
//            }
//        }
//        {
//            std::unique_lock<std::mutex> locker(windowMutex);
//            window.clear();
//            world.drawMap(&window);
//            uploadMapButton.draw(window);
//            window.display();
//        }
//    }
//}
//
//void moving(World &world) {
//    while (world.getNumberOfAnts() > 0) {
//        std::unique_lock<std::mutex> globalLock(windowMutex);
//        while (world.isPaused()) {
//            cv.wait(globalLock);
//        }
//        world.move();
//        globalLock.unlock();
//        cv.notify_one();
//        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//    }
//}