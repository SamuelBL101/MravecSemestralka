#include <iostream>
#include <SFML/Graphics.hpp>


int main(){
    int x = 100;
    int y = 100;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Langston's Ant");
    sf::Texture texture;
    if (!texture.loadFromFile("images/ant.png")) {
        std::cout << "Could not load enemy texture" << std::endl;
        return 0;
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        sf::Sprite image;
        image.setTexture(texture);
        window.clear();
        window.draw(image);
        window.display();

    }

    return 0;
}