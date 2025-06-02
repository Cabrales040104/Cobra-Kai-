#pragma once
#include <SFML/Graphics.hpp>

class Control
{
private:
    sf::Keyboard::Key arriba;
    sf::Keyboard::Key abajo;
    sf::Keyboard::Key derecha;
    sf::Keyboard::Key izquierda;

public:
    Control() : arriba(sf::Keyboard::Up), abajo(sf::Keyboard::Down), derecha(sf::Keyboard::Right), izquierda(sf::Keyboard::Left) {}
    Control(sf::Keyboard::Key arriba, sf::Keyboard::Key abajo, sf::Keyboard::Key derecha, sf::Keyboard::Key izquierda)
        : arriba(arriba), abajo(abajo), derecha(derecha), izquierda(izquierda) {}

    sf::Keyboard::Key moverArriba() { return arriba; }
    sf::Keyboard::Key moverAbajo() { return abajo; }
    sf::Keyboard::Key moverIzquierda() { return izquierda; }
    sf::Keyboard::Key moverDerecha() { return derecha; }
};