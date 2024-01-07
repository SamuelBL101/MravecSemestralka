//
// Created by olive on 2. 1. 2024.
//

#ifndef MRAVECSEMESTRALKA_BUTTON_H
#define MRAVECSEMESTRALKA_BUTTON_H

#include <iostream>
#include "SFML/Graphics.hpp"
/**
 * Class representing button
 */
class Button {
public:
    /**
     * Constructor
     * @param x x position of button
     * @param y y position of button
     * @param width width of button
     * @param height height of button
     * @param text text of button
     */
    Button(float x, float y, float width, float height, const std::string &text) {
        if (!font.loadFromFile("fonts/OpenSans-Light.ttf")) {
            std::cout << "Error loading font" << std::endl;
        }
        rect.setSize(sf::Vector2f(width, height));
        rect.setPosition(x, y);
        rect.setFillColor(sf::Color::Blue);

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(40);
        buttonText.setPosition(x - 10 , y + height);
        buttonText.setFillColor(sf::Color::Blue);

        isPressed = false;
    }
    /**
     * Draw button
     * @param window window where button will be drawn
     */
    void draw(sf::RenderWindow &window) {
        window.draw(rect);
        window.draw(buttonText);
    }

    /**
     * Check if mouse is over button
     * @param mousePos position of mouse
     * @return true if mouse is over button, false otherwise
     */
    bool isMouseOver(const sf::Vector2f &mousePos) {
        return rect.getGlobalBounds().contains(mousePos);
    }

    bool handleEvent(const sf::Event &event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            if (isMouseOver(mousePos)) {
                isPressed = !isPressed;
                rect.setFillColor(this->isPressed ? sf::Color::Green : sf::Color::Blue);
                return true;
            }
        }
        return false;
    }
    /**
     * Check if button is pressed
     * @return true if button is pressed, false otherwise
     */
    bool isPressedButton() {
        return this->isPressed;
    }
    /**
     * Set text of button
     * @param text text of button
     */
    void setText(const std::string &text) {
        buttonText.setString(text);
    }

private:
    sf::RectangleShape rect;
    sf::Text buttonText;
    sf::Font font;
    bool isPressed;
};


#endif //MRAVECSEMESTRALKA_BUTTON_H
