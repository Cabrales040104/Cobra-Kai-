#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>

class Campo {
    sf::Texture textura_;
    sf::Sprite  sprite_;
public:
    explicit Campo(const std::string& archivo) {
        if (!textura_.loadFromFile("assets/images/" + archivo))
            throw std::runtime_error("No se encontró la imagen: " + archivo);
        sprite_.setTexture(textura_);
    }
    void dibujar(sf::RenderWindow& w) const { w.draw(sprite_); }
};
