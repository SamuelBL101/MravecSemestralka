#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "World.h"
#include "my_socket.h"
#include <Windows.h>
#include "Button.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

std::mutex windowMutex;
std::condition_variable cv;

std::string convertFileToString(const std::string &filename) {
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

void convertStringToFile(const char *str) {
    std::istringstream iss(str);
    std::string filename;
    std::getline(iss, filename, ';');

    std::ofstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(iss, line, ';')) {
            file << line << '\n';
        }
        file.close();
    }
    std::cout << "File saved: " << filename << "\n";
}

void downloadMap(std::string &mapName, short port) {
    MySocket *mySocket = MySocket::createConnection("frios2.fri.uniza.sk", port);
    std::string message = "GET/" + mapName;
    std::cout << "Sending: " << message << std::endl;
    mySocket->sendData(message);
    std::string response = mySocket->receiveData();
    delete mySocket;
    mySocket = nullptr;
    convertStringToFile(response.c_str());
}

void uploadMap(World& world, std::string &filename, short port) {
    world.saveToFile(filename);
    MySocket *mySocket = MySocket::createConnection("frios2.fri.uniza.sk", port);
    std::string message = filename + ";" + convertFileToString(filename);
    mySocket->sendData(message);
    delete mySocket;
    mySocket = nullptr;
}

void antMovement(World &world) {
    sf::RenderWindow window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width, 0.9 * sf::VideoMode::getDesktopMode().height),
            "SFML Fullscreen Windowed");
    window.setPosition(sf::Vector2i(0, 0));
    Button uploadMapButton(20, sf::VideoMode::getDesktopMode().height * 0.85, 100, 50, "Upload Map");

    while (window.isOpen() && world.getNumberOfAnts() > 0) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (uploadMapButton.handleEvent(event)){
                std::string filename = "mapz.txt";
                uploadMap(world, filename, 18888);
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.x >= 0 && event.mouseButton.x <= world.getWidth() * world.getSizeOfBlock() &&
                        event.mouseButton.y >= 0 &&
                        event.mouseButton.y <= world.getHeight() * world.getSizeOfBlock()) {
                        int blockClickedX = std::floor(event.mouseButton.x / world.getSizeOfBlock());
                        int blockClickedY = std::floor(event.mouseButton.y / world.getSizeOfBlock());
                        world.changeBlockType(blockClickedX, blockClickedY);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 112) {
                        world.setPaused(!world.isPaused());
                        cv.notify_one();
                    }
                    if (event.text.unicode == 105) {
                        world.changeAntBehaviour();
                    }

                }
            }

            if (event.type == sf::Event::Closed) {
                window.close();
                cv.notify_one();
                if (world.getNumberOfAnts() <= 0)
                    break;
                else {
                    world.saveToFile("mapz.txt");
                    std::exit(0);
                }
            }
        }
        {
            std::unique_lock<std::mutex> locker(windowMutex);
            window.clear();
            world.drawMap(&window);
            uploadMapButton.draw(window);
            window.display();
        }
    }
}

void moving(World &world) {
    while (world.getNumberOfAnts() > 0) {
        std::unique_lock<std::mutex> globalLock(windowMutex);
        while (world.isPaused()) {
            cv.wait(globalLock);
        }
        world.move();
        globalLock.unlock();
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}



int main() {
//    const char *str = "suborrr.txt;3 3;0 1 0;1 1 1;0 0 0;";
//    std::string filename = "mapz.txt";
//    downloadMap(filename, 18888);

//    std::string filename2 = "mapz.txt";
//    uploadMap(filename2, 18888);
    bool loadMapFromFile = false;
    // FALSE = biela mapa, TRUE = random mapa
    bool typeOfMap = false;
    // 0 = one dies, 1 = all dies, 2 = change of logic
    int logics = 0;

    std::vector<Button> buttons;
    std::vector<int> colorsOfAnts;

    sf::RenderWindow window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Map editor");
    sf::Font font;
    font.loadFromFile("fonts/OpenSans-Light.ttf");

    Button loadMapButton(100, 100, 100, 50, "New Map");
    Button typeOfMapButton(100, 200, 100, 50, "White Map");

    Button behaviorButton1(100, 300, 100, 50, "One Dies");
    Button downloadMapButton(100, 400, 100, 50, "Download Map");


    std::string mapEditorString[4] = {"Enter number of ants: ", "Enter name of text file:", "Enter width of map: ",
                                      "Enter height of map: "};

    std::string parameters[4] = {"0", "", "0", "0"};
    int step = 0;

    sf::Text mapEditorText;
    mapEditorText.setFont(font);\
    mapEditorText.setString(mapEditorString[step]);
    mapEditorText.setCharacterSize(40);
    mapEditorText.setPosition(100, 50);
    mapEditorText.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            for (int i = 0; i < buttons.size(); ++i) {
                if (buttons.at(i).handleEvent(event)) {
                    colorsOfAnts.at(i)++;
                    colorsOfAnts.at(i) %= 5;
                    buttons.at(i).setText(to_string(static_cast<ColoredAnt>(colorsOfAnts.at(i))));
                }
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (loadMapButton.handleEvent(event)) {
                loadMapFromFile = loadMapButton.isPressedButton();
                loadMapButton.isPressedButton() ? loadMapButton.setText("Load Map") : loadMapButton.setText("New Map");
            } else if (typeOfMapButton.handleEvent(event)) {
                typeOfMap = typeOfMapButton.isPressedButton();
                if (typeOfMap) {
                    typeOfMapButton.setText("Random Map");
                } else {
                    typeOfMapButton.setText("White Map");
                }
            } else if (behaviorButton1.handleEvent(event)) {
                logics++;
                logics %= 3;
                switch (logics) {
                    case 0:
                        behaviorButton1.setText("One Dies");
                        break;
                    case 1:
                        behaviorButton1.setText("All Dies");
                        break;
                    case 2:
                        behaviorButton1.setText("Change of Logic");
                        break;
                    default:
                        break;
                }
            } else if(downloadMapButton.handleEvent(event)){
                //!stiahnutie mapy
                std::string filename = "mapz.txt";
                downloadMap(filename, 18888);
            }else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if (enteredChar == 13) {
                        step += 1;
                        if (step == 1) {
                            for (int i = 0; i < std::stoi(parameters[0]); ++i) {
                                Button button(100 + i * 150, 500, 100, 50, "Ant " + std::to_string(i));
                                buttons.push_back(button);
                                colorsOfAnts.push_back(0);
                            }
                        }
                        if (!loadMapFromFile && step == 1) {
                            step += 1;
                        }
                        mapEditorText.setString(mapEditorString[step % 4]);

                    }
                    if (enteredChar != 13)
                        parameters[step] += enteredChar;
                    mapEditorText.setString(mapEditorText.getString() + enteredChar);

                }
            }
        }
        if (loadMapFromFile && step > 1 || !loadMapFromFile && step > 3) {
            break;
        }
        window.clear(sf::Color::White);
//        if (parameters[2] != "" && parameters[3] != "") {
//            int width = std::stoi(parameters[2]);
//            int height = std::stoi(parameters[3]);
//            std::vector<std::vector<sf::RectangleShape>> rectMap(height, std::vector<sf::RectangleShape>(width));
//
//            for (int i = 0; i < height; ++i) {
//                for (int j = 0; j < width; ++j) {
//                    rectMap[i][j].setSize(sf::Vector2f(50.f, 50.f));
//                    rectMap[i][j].setPosition(50 + j * 50.f, 500 + i * 50.f);
//                    rectMap[i][j].setOutlineColor(sf::Color::Blue);
//                    rectMap[i][j].setOutlineThickness(2);
//                    window.draw(rectMap[i][j]);
//                }
//            }
//        }
        loadMapButton.draw(window);
        for (auto button: buttons) {
            button.draw(window);
        }
        typeOfMapButton.draw(window);
        behaviorButton1.draw(window);
        downloadMapButton.draw(window);
        window.draw(mapEditorText);
        window.display();
    }
    window.close();

    std::cout << parameters[1];
    int lower = sf::VideoMode::getDesktopMode().width < 0.85 * sf::VideoMode::getDesktopMode().height
                ? sf::VideoMode::getDesktopMode().width
                : sf::VideoMode::getDesktopMode().height * 0.85;

    if (loadMapFromFile) {
        World world(parameters[1], std::stoi(parameters[0]), lower);
        world.setAntsLogic(logics);
        std::thread tm1(antMovement, std::ref(world));
        std::thread tm2(moving, std::ref(world));

        tm1.join();
        tm2.join();
        world.saveToFile("mapz.txt");

    } else {
        int width = 20;
        width = std::stoi(parameters[2]);
        int height = 20;
        height = std::stoi(parameters[3]);
        float size =
                sf::VideoMode::getDesktopMode().width / width < 0.85 * sf::VideoMode::getDesktopMode().height / height
                ? sf::VideoMode::getDesktopMode().width / width
                : sf::VideoMode::getDesktopMode().height * 0.85 / height;
        int numberOfAnts = 3;
        numberOfAnts = std::stoi(parameters[0]);
        World world(width, height, numberOfAnts, typeOfMap, size);
        world.setAntsLogic(logics);
        for (int i = 0; i < numberOfAnts; ++i) {
            world.setAntColor(static_cast<ColoredAnt>(colorsOfAnts.at(i)), i);
        }

        std::thread tm1(antMovement, std::ref(world));
        std::thread tm2(moving, std::ref(world));

        tm1.join();
        tm2.join();
        world.saveToFile("mapz.txt");
    }
    return 0;
}
