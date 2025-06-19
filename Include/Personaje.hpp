#pragma once
#include <SFML/Graphics.hpp>
#include <Control.hpp>
#include <Vida.hpp>
#include <iostream>

class Personaje
{
private:
    float velocidad = 0.01f;
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
    sf::Sprite sprite;
    float escalaBase = 0.7f;
    bool atacando = false;
    bool puedeAtacar = true;

    Personaje(sf::Vector2f position, std::string imagen, Control control, sf::Vector2f healthBarPosition)
        : control(control), healthBar(100, healthBarPosition)
    {

        sf::Image img;
        if (!img.loadFromFile("assets/images/" + imagen))
        {
            throw std::runtime_error("No se pudo cargar la imagen: " + imagen);
        }

        for (unsigned int y = 0; y < img.getSize().y; ++y)
        {
            for (unsigned int x = 0; x < img.getSize().x; ++x)
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
            throw std::runtime_error("No se pudo crear la textura");
        }

        sprite.setTexture(textura);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

        sprite.setOrigin(frameWidth / 2.f, frameHeight);

        sprite.setScale(escalaBase, escalaBase);
        sprite.setPosition(position);
    }

    void mover(float offsetX)
    {
        sprite.move(offsetX, 0);
    }

    void aplicarEscala(bool mirandoIzquierda)
    {
        float escalaX = mirandoIzquierda ? -escalaBase : escalaBase;
        sprite.setScale(escalaX, escalaBase);
    }

    void leerTeclado(sf::Keyboard::Key teclaAtaque)
    {
        bool movio = false;

        if (sf::Keyboard::isKeyPressed(control.izquierda))
        {
            mover(-velocidad);
            aplicarEscala(true);
            movio = true;
        }
        else if (sf::Keyboard::isKeyPressed(control.derecha))
        {
            mover(velocidad);
            aplicarEscala(false);
            movio = true;
        }

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
            filaActual = 1;
        else if (!movio && !atacando)
            filaActual = 0;
    }

    void actualizar()
    {
        if (clock.getElapsedTime().asSeconds() >= frameTime)
        {
            cuadroActual = (cuadroActual + 1) % numFrames;
            int left = cuadroActual * frameWidth;
            int top = filaActual * frameHeight;

            if (sprite.getScale().x < 0)
                sprite.setTextureRect(sf::IntRect(left + frameWidth, top, -frameWidth, frameHeight));
            else
                sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));

            clock.restart();
        }
    }

    void dibujar(sf::RenderWindow &window)
    {
        window.draw(sprite);
        healthBar.dibujar(window);
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

    sf::FloatRect getHitbox() const
    {
        return sprite.getGlobalBounds();
    }
};
