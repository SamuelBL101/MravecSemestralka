#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "World.h"
#include <Windows.h>
#include "Button.h"
#include <string>

std::mutex windowMutex;
std::condition_variable cv;

void antMovement(World &world) {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Draw Map");


    while (window.isOpen() || world.getNumberOfAnts() > 0) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                cv.notify_one(); // Notify the waiting thread to exit
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
            window.display();
        }
    }
}

void moving(World &world) {
    while (world.getNumberOfAnts() > 0) {
        std::unique_lock<std::mutex> globalLock(windowMutex);
        world.move();
        globalLock.unlock();

        cv.notify_one();  // Notify the waiting thread to proceed
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    bool loadMapFromFile = false;
    // FALSE = biela mapa, TRUE = random mapa
    bool typeOfMap = false;
    // 0 = one dies, 1 = all dies, 2 = change of logic
    int logics = 0;

    std::vector<Button *> buttons;
    std::vector<int> colorsOfAnts;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Map editor");
    sf::Font font;
    font.loadFromFile("fonts/OpenSans-Light.ttf");

    Button loadMapButton(100, 100, 100, 50, "New Map");
    Button typeOfMapButton(100, 200, 100, 50, "White Map");

    Button behaviorButton1(100, 300, 100, 50, "One Dies");


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
                if (buttons.at(i)->handleEvent(event)) {
                    colorsOfAnts.at(i)++;
                    colorsOfAnts.at(i) %= 5;
                    buttons.at(i)->setText(to_string(static_cast<ColoredAnt>(colorsOfAnts.at(i))));
                    std::cout << colorsOfAnts.at(i) << std::endl;
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
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if (enteredChar == 13) {
                        step += 1;
                        if (step == 1) {
                            for (int i = 0; i < std::stoi(parameters[0]); ++i) {
                                buttons.push_back(new Button(100 + i * 150, 400, 100, 50, "Ant " + std::to_string(i)));
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
        loadMapButton.draw(window);
        for (auto &button: buttons) {
            button->draw(window);
        }
        typeOfMapButton.draw(window);
        behaviorButton1.draw(window);
        window.draw(mapEditorText);
        window.display();
    }
    window.close();

    std::cout << "hi";
    std::cout << parameters[1];
    std::cout << "hi";
    if (loadMapFromFile) {
        World world(parameters[1], std::stoi(parameters[0]));
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
        int numberOfAnts = 3;
        numberOfAnts = std::stoi(parameters[0]);
        World world(width, height, numberOfAnts, typeOfMap);
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
