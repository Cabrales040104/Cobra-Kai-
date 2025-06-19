#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>
#include <iostream>

class Campo
{
    sf::Texture textura_;
    sf::Sprite sprite_;

public:
    explicit Campo(const std::string &archivo)
    {
        if (!textura_.loadFromFile("assets/images/" + archivo))
        {
            std::cerr << "Error al cargar la imagen: " << archivo << std::endl;
            throw std::runtime_error("No se encontró la imagen: " + archivo);
        }
        sprite_.setTexture(textura_);
        sprite_.setOrigin(0, 0);
        sprite_.setPosition(0, 0);
        sprite_.setScale(0.1f, 0.1f);
    }

    void dibujar(sf::RenderWindow &w) const { w.draw(sprite_); }

    void cambiarImagen(const std::string &archivo, sf::Vector2u windowSize = {0, 0})
    {
        if (!textura_.loadFromFile("assets/images/" + archivo))
        {
            std::cerr << "Error al cargar la imagen: " << archivo << std::endl;
            throw std::runtime_error("No se encontró la imagen: " + archivo);
        }
        sprite_.setTexture(textura_);
        sprite_.setOrigin(0, 0);
        sprite_.setPosition(0, 0);

        if (windowSize.x > 0 && windowSize.y > 0)
        {
            sf::Vector2u textureSize = textura_.getSize();
            sprite_.setScale(
                float(windowSize.x) / textureSize.x,
                float(windowSize.y) / textureSize.y);
        }
        else
        {
            sprite_.setScale(0.1f, 0.1f);
        }
    }
};
