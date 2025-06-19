#pragma once
#include <SFML/Graphics.hpp>
#include <Control.hpp>
#include <Vida.hpp>
#include <iostream>

class Personaje
{
private:
    double velocidad = 0.1;
    sf::Texture textura;
    sf::Clock clock;
    float frameTime = 0.1f;

    int cuadroActual = 0;
    int numFrames = 8;
    int frameWidth = 128;
    int frameHeight = 256;
    int filaActual = 0;
    Control control;
    Vida healthBar;
    int score = 0;

public:
    float escalaBase = 0.1f; // Escala base del sprite
    bool atacando = false;
    bool puedeAtacar = true;

    sf::Sprite sprite;

    Personaje(sf::Vector2f position, std::string imagen, Control control, sf::Vector2f healthBarPosition)
        : control(control), healthBar(100, healthBarPosition)
    {
        sf::Image img;
        if (!img.loadFromFile("assets/images/" + imagen))
        {
            throw "No se pudo cargar la imagen: ";
        }
        sf::Vector2u size = img.getSize();
        for (unsigned int y = 0; y < size.y; ++y)
        {
            for (unsigned int x = 0; x < size.x; ++x)
            {
                sf::Color c = img.getPixel(x, y);
                if (abs(int(c.r) - 178) < 21 && abs(int(c.g) - 255) < 21 && abs(int(c.b) - 178) < 21)
                {
                    img.setPixel(x, y, sf::Color(0, 0, 0, 0));
                }
            }
        }
        if (!textura.loadFromImage(img))
        {
            throw "No se pudo crear la textura";
        }
        sprite.setTexture(textura);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2, bounds.height / 2);
        sprite.setPosition(position.x, 654);
        sprite.setScale(1.f, 1.f); // Escala base
    }

    void mover(float offsetX)
    {
        sprite.move(offsetX, 0);
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
            int left = cuadroActual * frameWidth;
            int top = filaActual * frameHeight;

            sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));
            sprite.setPosition(sprite.getPosition().x, 654);
            clock.restart();
        }
    }

    void leerTeclado(sf::Keyboard::Key teclaAtaque)
    {
        bool movio = false;

        if (sf::Keyboard::isKeyPressed(control.izquierda))
        {
            mover(-velocidad);
            sprite.setScale(-1.f, 1.f); // Voltear sprite horizontalmente
            movio = true;
        }
        else if (sf::Keyboard::isKeyPressed(control.derecha))
        {
            mover(velocidad);
            sprite.setScale(1.f, 1.f); // Restaurar orientación
            movio = true;
            sprite.setPosition(sprite.getPosition().x, 654);
        }

        // Ataque
        if (sf::Keyboard::isKeyPressed(teclaAtaque) && puedeAtacar)
        {
            atacando = true;
            puedeAtacar = false;
            filaActual = 2;
        }

        if (!sf::Keyboard::isKeyPressed(teclaAtaque))
        {
            puedeAtacar = true;
        }

        if (movio && !atacando)
        {
            filaActual = 1;
        }
        else if (!movio && !atacando)
        {
            filaActual = 0;
        }
        sprite.setPosition(sprite.getPosition().x, 654);
        std::cout << "Posición Y: " << sprite.getPosition().y << std::endl;
    }

    sf::FloatRect getHitbox() const
    {
        return sf::FloatRect(
            sprite.getPosition().x - 32,
            sprite.getPosition().y - 128,
            64,
            128
        );
    }

    void takeDamage(int damage)
    {
        healthBar.takeDamage(damage);
        filaActual = 3;
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