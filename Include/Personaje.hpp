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
    int numFrames = 8;           
    int frameWidth = 64;         
    int frameHeight = 64;        

    int filaActual = 0;          
    Control control;
    Vida healthBar;
    int score = 0;

public:
    bool atacando = false;
    bool puedeAtacar = true;
    bool defendiendo = false;  // <-- Nueva variable para defensa

    sf::Sprite sprite;

    Personaje(sf::Vector2f position, std::string imagen, Control control, sf::Vector2f healthBarPosition)
        : control(control), healthBar(100, healthBarPosition)  // 100 de vida, 10 golpes de 10
    {
       sf::Image img;
       if (!img.loadFromFile("assets/images/" + imagen)) {
          throw "No se pudo cargar la imagen: ";
        }
        sf::Vector2u size = img.getSize();
        for (unsigned int y = 0; y < size.y; ++y) {
             for (unsigned int x = 0; x < size.x; ++x) {
             sf::Color c = img.getPixel(x, y);
             if (abs(int(c.r) - 178) < 21 && abs(int(c.g) - 255) < 21 && abs(int(c.b) - 178) < 21) {
                 img.setPixel(x, y, sf::Color(0, 0, 0, 0)); 
             }
         }
        }
        if (!texture.loadFromImage(img)){
           throw "No se pudo crear la textura";
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setOrigin(frameWidth / 2.f, frameHeight); 
        sprite.setPosition(position.x, 654); 
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

    // Cambié este método para que controle defensa y ataque
    void leerTeclado(sf::Keyboard::Key teclaAtaque, sf::Keyboard::Key teclaDefensa)
    {
        bool movio = false;
        defendiendo = false;  // reset defensa cada frame

        if (sf::Keyboard::isKeyPressed(control.moverIzquierda()))
        {
            mover(-velocidad);
            sprite.setScale(-1.f, 1.f);
            movio = true;
        }
        else if (sf::Keyboard::isKeyPressed(control.moverDerecha()))
        {
            mover(velocidad);
            sprite.setScale(1.f, 1.f);
            movio = true;
            sprite.setPosition(sprite.getPosition().x, 654);
        }

        // Defensa: si está presionada la tecla de defensa
        if (sf::Keyboard::isKeyPressed(teclaDefensa))
        {
            defendiendo = true;
            filaActual = 4; // Por ejemplo, fila 4 para animación de defensa
        }
        else if (sf::Keyboard::isKeyPressed(teclaAtaque) && puedeAtacar && !defendiendo)
        {
            atacando = true;
            puedeAtacar = false;
            filaActual = 1;  // fila ataque
        }

        if (!sf::Keyboard::isKeyPressed(teclaAtaque))
        {
            puedeAtacar = true;
            atacando = false;
        }

        if (movio && !atacando && !defendiendo)
        {
            filaActual = 1;  // fila caminar
        }
        else if (!movio && !atacando && !defendiendo)
        {
            filaActual = 0;  // idle
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
        if (!defendiendo)  // solo resta vida si NO está defendiendo
        {
            healthBar.takeDamage(damage);
            filaActual = 3; // fila daño
        }
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
