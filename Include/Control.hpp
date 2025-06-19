#pragma once
#include <SFML/Window/Keyboard.hpp>

class Control
{
public:
    sf::Keyboard::Key derecha, izquierda, ataque, defensa;

    Control(

        sf::Keyboard::Key derecha,
        sf::Keyboard::Key izquierda,
        sf::Keyboard::Key ataque,
        sf::Keyboard::Key defensa) : derecha(derecha), izquierda(izquierda), ataque(ataque), defensa(defensa) {}

    Control() = default;
};
