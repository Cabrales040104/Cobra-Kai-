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
    int numFrames = 8;           // ✅ Hay 8 cuadros por fila
    int frameWidth = 48;         // ✅ Ancho correcto del cuadro en el spritesheet
    int frameHeight = 48;        // ✅ Alto correcto del cuadro en el spritesheet

    int filaActual = 0;          // Fila actual del spritesheet (cambiar según animación)
    Control control;
    Vida healthBar;
    int score = 0;

public:
    bool atacando = false;
    bool puedeAtacar = true;

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

        // ✅ Mostrar el primer cuadro del spritesheet
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

        // ✅ Ajustar el origen al centro inferior del cuadro
        sprite.setOrigin(frameWidth / 2.f, frameHeight);
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
        // Animación simple: recorre los cuadros de la fila actual
        if (clock.getElapsedTime().asSeconds() >= frameTime)
        {
            cuadroActual = (cuadroActual + 1) % numFrames;
            int left = cuadroActual * frameWidth;
            int top = filaActual * frameHeight;

            sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));
            clock.restart();
        }
    }

    void leerTeclado(sf::Keyboard::Key teclaAtaque)
    {
        bool movio = false;

        if (sf::Keyboard::isKeyPressed(control.moverIzquierda()))
        {
            mover(-velocidad, 0);
            sprite.setScale(-1.f, 1.f); // Voltear sprite horizontalmente
            movio = true;
        }
        else if (sf::Keyboard::isKeyPressed(control.moverDerecha()))
        {
            mover(velocidad, 0);
            sprite.setScale(1.f, 1.f);  // Restaurar orientación
            movio = true;
        }

        if (sf::Keyboard::isKeyPressed(control.moverArriba()))
        {
            mover(0, -velocidad);
            movio = true;
        }
        if (sf::Keyboard::isKeyPressed(control.moverAbajo()))
        {
            mover(0, velocidad);
            movio = true;
        }

        // Ataque
        if (sf::Keyboard::isKeyPressed(teclaAtaque) && puedeAtacar)
        {
            atacando = true;
            puedeAtacar = false;
            filaActual = 2;  // ✅ Puedes cambiar esto según la fila de animación de ataque
        }

        // Permitir ataque nuevamente al soltar tecla
        if (!sf::Keyboard::isKeyPressed(teclaAtaque))
        {
            puedeAtacar = true;
        }

        // Si se está moviendo, usar fila de movimiento (opcional)
        if (movio && !atacando)
        {
            filaActual = 1;  // ✅ Suponiendo que la fila 1 es de caminar
        }
        else if (!movio && !atacando)
        {
            filaActual = 0;  // ✅ Fila de idle/inactivo
        }
    }

    void takeDamage(int damage)
    {
        healthBar.takeDamage(damage);
        filaActual = 3; // ✅ Por ejemplo, fila 3 para daño
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
