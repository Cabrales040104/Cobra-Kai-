#pragma once
#include <SFML/Graphics.hpp>
#include "Control.hpp"
#include "Vida.hpp"
#include <iostream>

class Personaje
{
private:
    float velocidad = 150.0f;

    sf::Texture texturaDerecha;
    sf::Texture texturaIzquierda;
    sf::Clock clockAnimacion;
    sf::Clock clockMovimiento;
    float frameTime = 0.1f;

    int cuadroActual = 0;
    int numFrames = 8;
    int frameWidth = 128;
    int frameHeight = 256;
    int filaActual = 0;

    Control control;
    Vida barraVida;

    sf::Clock clockAtaque;
    sf::Clock clockDaño;
    float tiempoAtaque = 0.5f;
    float tiempoDaño = 0.3f;
    bool recibiendoDaño = false;
    bool mirandoIzquierda = false;

    std::string imagenDerecha;
    std::string imagenIzquierda;
    bool usarImagenAlternativa = false;

    void cargarTextura(const std::string &imagen, sf::Texture &textura)
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
                sf::Color pixel = img.getPixel(x, y);
                if (abs(int(pixel.r) - 178) < 21 &&
                    abs(int(pixel.g) - 255) < 21 &&
                    abs(int(pixel.b) - 178) < 21)
                {
                    img.setPixel(x, y, sf::Color(0, 0, 0, 0));
                }
            }
        }

        if (!textura.loadFromImage(img))
        {
            throw std::runtime_error("No se pudo crear la textura");
        }
    }

    void configurarSprite(sf::Vector2f position)
    {
        sprite.setTexture(texturaDerecha);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setOrigin(frameWidth / 2.0f, frameHeight);
        sprite.setScale(escalaBase, escalaBase);
        sprite.setPosition(position);
    }

    void cambiarTexturaSegunDireccion()
    {

        if (mirandoIzquierda && !imagenIzquierda.empty())
        {
            sprite.setTexture(texturaIzquierda);
        }
        else
        {
            sprite.setTexture(texturaDerecha);
        }
        sprite.setTexture(texturaDerecha);
    }

public:
    sf::Sprite sprite;
    float escalaBase = 0.7f;
    bool atacando = false;
    bool puedeAtacar = true;

    Personaje(sf::Vector2f position, std::string imagen, Control ctrl, sf::Vector2f healthBarPosition)
        : control(ctrl), barraVida(100, healthBarPosition), imagenDerecha(imagen)
    {
        cargarTextura(imagen, texturaDerecha);
        configurarSprite(position);

        mirandoIzquierda = false;
    }

    void cargarImagenIzquierda(const std::string &imagen)
    {
        imagenIzquierda = imagen;
        cargarTextura(imagen, texturaIzquierda);
        usarImagenAlternativa = true;
    }

    void mover(float deltaTime, float direccion)
    {
        float desplazamiento = velocidad * deltaTime * direccion;
        sprite.move(desplazamiento, 0);

        bool nuevaDireccion = (direccion < 0);

        if (nuevaDireccion != mirandoIzquierda)
        {
            mirandoIzquierda = nuevaDireccion;
            cambiarTexturaSegunDireccion();
            aplicarEscala();
        }
    }

    void aplicarEscala()
    {

        if (usarImagenAlternativa && mirandoIzquierda)
        {
            sprite.setScale(escalaBase, escalaBase);
        }
        else
        {

            float escalaX = mirandoIzquierda ? -escalaBase : escalaBase;
            sprite.setScale(escalaX, escalaBase);
        }
    }

    void voltearHacia(bool izquierda)
    {
        if (izquierda != mirandoIzquierda)
        {
            mirandoIzquierda = izquierda;
            cambiarTexturaSegunDireccion();
            aplicarEscala();
        }
    }

    void leerTeclado(sf::Keyboard::Key teclaAtaque)
    {
        if (recibiendoDaño)
            return;

        float deltaTime = clockMovimiento.restart().asSeconds();
        bool movio = false;

        if (sf::Keyboard::isKeyPressed(control.izquierda))
        {
            mover(deltaTime, -1.0f);
            movio = true;
        }
        else if (sf::Keyboard::isKeyPressed(control.derecha))
        {
            mover(deltaTime, 1.0f);
            movio = true;
        }

        if (sf::Keyboard::isKeyPressed(teclaAtaque) && puedeAtacar && !atacando)
        {
            iniciarAtaque();
        }

        if (atacando)
        {
            filaActual = 2;
        }
        else if (movio)
        {
            filaActual = 1;
        }
        else
        {
            filaActual = 0;
        }
    }

    void iniciarAtaque()
    {
        atacando = true;
        puedeAtacar = false;
        clockAtaque.restart();
        filaActual = 2;
    }

    void actualizar()
    {

        if (atacando && clockAtaque.getElapsedTime().asSeconds() >= tiempoAtaque)
        {
            atacando = false;
            puedeAtacar = true;
        }

        if (recibiendoDaño && clockDaño.getElapsedTime().asSeconds() >= tiempoDaño)
        {
            recibiendoDaño = false;
        }

        actualizarAnimacion();
    }

    void actualizarAnimacion()
    {
        if (clockAnimacion.getElapsedTime().asSeconds() >= frameTime)
        {
            cuadroActual = (cuadroActual + 1) % numFrames;

            int left = cuadroActual * frameWidth;
            int top = filaActual * frameHeight;

            sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));

            clockAnimacion.restart();
        }
    }

    void takeDamage(int damage)
    {
        barraVida.takeDamage(damage);
        recibiendoDaño = true;
        filaActual = 3;
        clockDaño.restart();
    }

    void dibujar(sf::RenderWindow &window)
    {
        window.draw(sprite);
        barraVida.dibujar(window);
    }

    int getHealth() const { return barraVida.getCurrentHealth(); }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::FloatRect getHitbox() const
    {
        sf::FloatRect bounds = sprite.getGlobalBounds();

        bounds.left += bounds.width * 0.1f;
        bounds.width *= 0.8f;
        bounds.top += bounds.height * 0.1f;
        bounds.height *= 0.9f;
        return bounds;
    }

    bool estaAtacando() const { return atacando; }
    sf::Vector2f getPosicion() const { return sprite.getPosition(); }
    bool estaMirandoIzquierda() const { return mirandoIzquierda; }

    void habilitarImagenAlternativa(bool habilitar) { usarImagenAlternativa = habilitar; }
    bool tieneImagenAlternativa() const { return usarImagenAlternativa && !imagenIzquierda.empty(); }
};