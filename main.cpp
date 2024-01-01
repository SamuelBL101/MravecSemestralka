#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "World.h"

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
                //std::exit(0);
                break;
            }
        }

        {
            std::unique_lock<std::mutex> locker(windowMutex);
            window.clear();
            world.drawMap(&window);
            window.display();
        }
        // Other background tasks
    }
}

void moving(World &world) {
    while (world.getNumberOfAnts() > 0) {
        std::unique_lock<std::mutex> globalLock(windowMutex);
        world.move();
        globalLock.unlock();

        cv.notify_one();  // Notify the waiting thread to proceed
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::cout << "Chcete nacitat alebo vytvorit mapu" << std::endl;
    int choice = 0;
    std::cin >> choice;
    if (choice == 1) {
        std::string fileName = "mapz.txt";
        std::cout << "Enter file name: ";
        std::cin >> fileName;
        int numberOfAnts = 3;
        std::cout << "Enter number of ants: ";
        std::cin >> numberOfAnts;
        World world(fileName, numberOfAnts);
        std::thread tm1(antMovement, std::ref(world));
        std::thread tm2(moving, std::ref(world));

        tm1.join();
        tm2.join();
        world.saveToFile("mapz.txt");
    } else {
        int width = 20;
        std::cout << "Enter width of map: ";
        std::cin >> width;
        int height = 20;
        std::cout << "Enter height of map: ";
        std::cin >> height;
        int numberOfAnts = 3;
        std::cout << "Enter number of ants: ";
        std::cin >> numberOfAnts;
        World world(width, height, numberOfAnts);

        std::thread tm1(antMovement, std::ref(world));
        std::thread tm2(moving, std::ref(world));

        tm1.join();
        tm2.join();
        world.saveToFile("mapz.txt");
    }
    return 0;
}
