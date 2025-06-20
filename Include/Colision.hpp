#pragma once
#include <SFML/Graphics.hpp>
#include "Personaje.hpp"
#include <cmath>

class Colision
{
private:
    static sf::Clock clockDaño1;
    static sf::Clock clockDaño2;
    static const float TIEMPO_ENTRE_DAÑOS;

    static void separarPersonajes(Personaje &p1, Personaje &p2)
    {
        sf::FloatRect bounds1 = p1.getHitbox();
        sf::FloatRect bounds2 = p2.getHitbox();
        sf::FloatRect interseccion;

        if (bounds1.intersects(bounds2, interseccion))
        {

            sf::Vector2f pos1 = p1.getPosicion();
            sf::Vector2f pos2 = p2.getPosicion();

            float dx = pos1.x - pos2.x;
            float separacion = interseccion.width / 2.0f + 1.0f;

            if (dx > 0)
            {
                p1.sprite.move(separacion, 0);
                p2.sprite.move(-separacion, 0);
            }
            else
            {
                p1.sprite.move(-separacion, 0);
                p2.sprite.move(separacion, 0);
            }
        }
    }

    static void manejarAtaques(Personaje &p1, Personaje &p2)
    {
        const int DAÑO_ATAQUE = 5;

        if (p1.estaAtacando() && clockDaño1.getElapsedTime().asSeconds() >= TIEMPO_ENTRE_DAÑOS)
        {
            p2.takeDamage(DAÑO_ATAQUE);
            clockDaño1.restart();
        }

        if (p2.estaAtacando() && clockDaño2.getElapsedTime().asSeconds() >= TIEMPO_ENTRE_DAÑOS)
        {
            p1.takeDamage(DAÑO_ATAQUE);
            clockDaño2.restart();
        }
    }

public:
    static bool detectar(const Personaje &p1, const Personaje &p2)
    {
        return p1.getHitbox().intersects(p2.getHitbox());
    }

    static void manejar(Personaje &p1, Personaje &p2)
    {
        if (!detectar(p1, p2))
            return;

        separarPersonajes(p1, p2);

        manejarAtaques(p1, p2);
    }
};

sf::Clock Colision::clockDaño1;
sf::Clock Colision::clockDaño2;
const float Colision::TIEMPO_ENTRE_DAÑOS = 0.5f;