#pragma once
#include <SFML/Graphics.hpp>
#include <Personaje.hpp>

class Colision
{
public:
    static bool detectar(Personaje &p1, Personaje &p2)
    {
        return p1.getBounds().intersects(p2.getBounds());
    }

    static void manejar(Personaje &p1, Personaje &p2)
    {
        if (detectar(p1, p2))
        {
            sf::FloatRect interseccion;
            p1.getBounds().intersects(p2.getBounds(), interseccion);

            if (interseccion.width < interseccion.height)
            {
                if (p1.sprite.getPosition().x < p2.sprite.getPosition().x)
                {
                    p1.mover(-interseccion.width / 2, 0);
                    p2.mover(interseccion.width / 2, 0);
                }
                else
                {
                    p1.mover(interseccion.width / 2, 0);
                    p2.mover(-interseccion.width / 2, 0);
                }
            }
            else
            {
                if (p1.sprite.getPosition().y < p2.sprite.getPosition().y)
                {
                    p1.mover(0, -interseccion.height / 2);
                    p2.mover(0, interseccion.height / 2);
                }
                else
                {
                    p1.mover(0, interseccion.height / 2);
                    p2.mover(0, -interseccion.height / 2);
                }
            }

            if (p1.atacando)
                p2.takeDamage(1);
            if (p2.atacando)
                p1.takeDamage(1);
        }
    }
};