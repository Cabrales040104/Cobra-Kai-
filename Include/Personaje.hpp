#pragma once
#include <SFML/Graphics.hpp>
#include "Control.hpp"
#include "Vida.hpp"
#include <iostream>

class Personaje
{
private:
    // Propiedades de movimiento
    float velocidad = 150.0f; // pixels por segundo
    
    // Propiedades de animación
    sf::Texture textura;
    sf::Clock clockAnimacion;
    sf::Clock clockMovimiento;
    float frameTime = 0.1f;
    
    int cuadroActual = 0;
    int numFrames = 8;
    int frameWidth = 128;
    int frameHeight = 256;
    int filaActual = 0; // 0: idle, 1: caminar, 2: atacar, 3: recibir daño
    
    // Control y vida
    Control control;
    Vida barraVida;
    
    // Estado del personaje
    sf::Clock clockAtaque;
    sf::Clock clockDaño;
    float tiempoAtaque = 0.5f; // duración del ataque
    float tiempoDaño = 0.3f;   // duración de la animación de daño
    bool recibiendoDaño = false;

    void cargarTextura(const std::string& imagen)
    {
        sf::Image img;
        if (!img.loadFromFile("assets/images/" + imagen))
        {
            throw std::runtime_error("No se pudo cargar la imagen: " + imagen);
        }
        
        // Remover fondo verde claro (chroma key)
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
        sprite.setTexture(textura);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setOrigin(frameWidth / 2.0f, frameHeight);
        sprite.setScale(escalaBase, escalaBase);
        sprite.setPosition(position);
    }

public:
    sf::Sprite sprite;
    float escalaBase = 0.7f;
    bool atacando = false;
    bool puedeAtacar = true;
    
    Personaje(sf::Vector2f position, std::string imagen, Control ctrl, sf::Vector2f healthBarPosition)
        : control(ctrl), barraVida(100, healthBarPosition)
    {
        cargarTextura(imagen);
        configurarSprite(position);
    }
    
    void mover(float deltaTime, float direccion)
    {
        float desplazamiento = velocidad * deltaTime * direccion;
        sprite.move(desplazamiento, 0);
        
        // Aplicar escala según la dirección
        bool mirandoIzquierda = (direccion < 0);
        aplicarEscala(mirandoIzquierda);
    }
    
    void aplicarEscala(bool mirandoIzquierda)
    {
        float escalaX = mirandoIzquierda ? -escalaBase : escalaBase;
        sprite.setScale(escalaX, escalaBase);
    }
    
    void leerTeclado(sf::Keyboard::Key teclaAtaque)
    {
        if (recibiendoDaño) return; // No puede moverse mientras recibe daño
        
        float deltaTime = clockMovimiento.restart().asSeconds();
        bool movio = false;
        
        // Movimiento
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
        
        // Ataque
        if (sf::Keyboard::isKeyPressed(teclaAtaque) && puedeAtacar && !atacando)
        {
            iniciarAtaque();
        }
        
        // Determinar animación
        if (atacando)
        {
            filaActual = 2; // animación de ataque
        }
        else if (movio)
        {
            filaActual = 1; // animación de caminar
        }
        else
        {
            filaActual = 0; // animación idle
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
        // Actualizar estado de ataque
        if (atacando && clockAtaque.getElapsedTime().asSeconds() >= tiempoAtaque)
        {
            atacando = false;
            puedeAtacar = true;
        }
        
        // Actualizar estado de daño
        if (recibiendoDaño && clockDaño.getElapsedTime().asSeconds() >= tiempoDaño)
        {
            recibiendoDaño = false;
        }
        
        // Actualizar animación
        actualizarAnimacion();
    }
    
    void actualizarAnimacion()
    {
        if (clockAnimacion.getElapsedTime().asSeconds() >= frameTime)
        {
            cuadroActual = (cuadroActual + 1) % numFrames;
            
            int left = cuadroActual * frameWidth;
            int top = filaActual * frameHeight;
            
            // Invertir textura si está mirando a la izquierda
            if (sprite.getScale().x < 0)
            {
                sprite.setTextureRect(sf::IntRect(left + frameWidth, top, -frameWidth, frameHeight));
            }
            else
            {
                sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));
            }
            
            clockAnimacion.restart();
        }
    }
    
    void takeDamage(int damage)
    {
        barraVida.takeDamage(damage);
        recibiendoDaño = true;
        filaActual = 3; // animación de recibir daño
        clockDaño.restart();
    }
    
    void dibujar(sf::RenderWindow& window)
    {
        window.draw(sprite);
        barraVida.dibujar(window);
    }
    
    // Getters
    int getHealth() const { return barraVida.getCurrentHealth(); }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::FloatRect getHitbox() const 
    { 
        sf::FloatRect bounds = sprite.getGlobalBounds();
        // Reducir ligeramente el hitbox para colisiones más precisas
        bounds.left += bounds.width * 0.1f;
        bounds.width *= 0.8f;
        bounds.top += bounds.height * 0.1f;
        bounds.height *= 0.9f;
        return bounds;
    }
    
    bool estaAtacando() const { return atacando; }
    sf::Vector2f getPosicion() const { return sprite.getPosition(); }
};