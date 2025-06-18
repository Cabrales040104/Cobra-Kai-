#pragma once
#include <SFML/Window/Keyboard.hpp>

class Control {
public:
    sf::Keyboard::Key arriba, abajo, derecha, izquierda, ataque, defensa;

    Control(
        sf::Keyboard::Key arriba,
        sf::Keyboard::Key abajo,
        sf::Keyboard::Key derecha,
        sf::Keyboard::Key izquierda,
        sf::Keyboard::Key ataque,
        sf::Keyboard::Key defensa
    ) : arriba(arriba), abajo(abajo), derecha(derecha), izquierda(izquierda), ataque(ataque), defensa(defensa) {}

    Control() = default;
};




