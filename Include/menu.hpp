#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>

class Menu
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Font font; // Esta sigue siendo privada
    sf::Text text;
    std::vector<std::string> imagePaths;
    int currentImageIndex = 0;
    sf::Clock clock;
    float switchTime = 1.0f;

public:
    Menu(std::vector<std::string> imagenes)
    {
        imagePaths = imagenes;

        if (!texture.loadFromFile("assets/images/" + imagePaths[currentImageIndex]))
        {
            throw "No se encontró imagen";
        }
        sprite = sf::Sprite(texture);

        if (!font.loadFromFile("assets/fonts/Dead Stock.ttf"))
        {
            throw "No se pudo cargar la fuente";
        }

        text.setFont(font);
        text.setString("Bienvenido a Cobra Kai\nPresiona Enter para jugar");
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        text.setPosition(120, 180);
    }

    void dibujar(sf::RenderWindow &window)
    {
        window.draw(this->sprite);
        window.draw(text);
    }

    bool iniciarJuego()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            return true;
        }
        return false;
    }

    void actualizar()
    {
        if (clock.getElapsedTime().asSeconds() >= switchTime)
        {
            currentImageIndex = (currentImageIndex + 1) % imagePaths.size();

            if (!texture.loadFromFile("assets/images/" + imagePaths[currentImageIndex]))
            {
                throw "No se encontró la imagen";
            }
            sprite.setTexture(texture);

            clock.restart();
        }
    }

    // Método público para obtener la fuente
    sf::Font &getFont()
    {
        return font;
    }
};

 
 

