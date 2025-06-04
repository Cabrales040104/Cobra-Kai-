#pragma once
#include <SFML/Graphics.hpp>
#include <Control.hpp>
#include <Vida.hpp>
#include <iostream>

class Personaje
{
private:
    double velocidad = 0.1;
    sf::Texture texture;
    sf::Clock clock;
    float frameTime = 0.1f;
    int cuadroActual = 0;
    int numFrames = 4;
    int frameWidth = 32;
    int frameHeight = 32;
    Control control;
    Vida healthBar;
    int score = 0;

public:
    bool atacando = false;
    bool puedeAtacar = true; // Nuevo flag para controlar el ataque

    sf::Sprite sprite;

    Personaje(sf::Vector2f position, std::string imagen, Control control, sf::Vector2f healthBarPosition)
        : control(control), healthBar(100, healthBarPosition)
    {
        if (!texture.loadFromFile("assets/images/" + imagen))
        {
            throw "No se encontró imagen";
        }
        sprite = sf::Sprite(texture);
        sprite.setPosition(position);
    }

    void mover(float offsetX, float offsetY)
    {
        sprite.move(offsetX, offsetY);
    }

    void dibujar(sf::RenderWindow &window)
    {
        window.draw(sprite);
        healthBar.dibujar(window);
    }

    void actualizar()
    {
        if (clock.getElapsedTime().asSeconds() >= frameTime)
        {
            cuadroActual = (cuadroActual + 1) % numFrames;
            sprite.setTextureRect(sf::IntRect((cuadroActual * 64) + 17, 133, 64, 36));
            clock.restart();
        }
        if (clock.getElapsedTime().asSeconds() >= frameTime)
        {
            cuadroActual = (cuadroActual + 1) % numFrames; // Cambia al siguiente cuadro
            int left = cuadroActual * frameWidth;          // Coordenada x del cuadro actual
            int top = 0;                                   // Coordenada y (fila 0)
            sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));
            clock.restart();
        }
    }

    void leerTeclado(sf::Keyboard::Key teclaAtaque)
    {
        if (sf::Keyboard::isKeyPressed(control.moverIzquierda()))
        {
            mover(-velocidad, 0);
            sprite.setScale(-1.f, 1.f); // Voltear el sprite horizontalmente
        }
        else if (sf::Keyboard::isKeyPressed(control.moverDerecha()))
        {
            mover(velocidad, 0);
            sprite.setScale(1.f, 1.f); // Restaurar el sprite a su orientación original
        }

        if (sf::Keyboard::isKeyPressed(control.moverArriba()))
            mover(0, -velocidad);
        if (sf::Keyboard::isKeyPressed(control.moverAbajo()))
            mover(0, velocidad);

        // Solo atacar si la tecla de ataque está presionada y el personaje puede atacar
        if (sf::Keyboard::isKeyPressed(teclaAtaque) && puedeAtacar)
        {
            atacando = true;
            puedeAtacar = false; // Desactivar ataque hasta que se suelte la tecla
        }

        // Permitir que el personaje pueda atacar de nuevo cuando se suelta la tecla
        if (!sf::Keyboard::isKeyPressed(teclaAtaque))
        {
            puedeAtacar = true; // Permitir el siguiente ataque
        }
    }

    void takeDamage(int damage)
    {
        healthBar.takeDamage(damage);
    }

    int getHealth() const
    {
        return healthBar.getCurrentHealth();
    }

    sf::FloatRect getBounds() const
    {
        return sprite.getGlobalBounds();
    }
};