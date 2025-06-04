#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream> 

class Control 
{
private:
    std::map<std::string, sf::Keyboard::Key> teclas; 

public:
    Control()
    {
        teclas["arriba"] = sf::Keyboard::Up;
        teclas["abajo"] = sf::Keyboard::Down;
        teclas["derecha"] = sf::Keyboard::Right;
        teclas["izquierda"] = sf::Keyboard::Left;
        teclas["ataque"] = sf::Keyboard::A;
        teclas["defensa"] = sf::Keyboard::S;
        teclas["salto"] = sf::Keyboard::Space;
        teclas["especial"] = sf::Keyboard::D;
    }

    Control(sf::Keyboard::Key arriba, sf::Keyboard::Key abajo, 
           sf::Keyboard::Key derecha, sf::Keyboard::Key izquierda, 
           sf::Keyboard::Key ataque, sf::Keyboard::Key defensa, 
           sf::Keyboard::Key salto, sf::Keyboard::Key especial)
    {
        teclas["arriba"] = arriba;
        teclas["abajo"] = abajo;
        teclas["derecha"] = derecha;
        teclas["izquierda"] = izquierda;
        teclas["ataque"] = ataque;
        teclas["defensa"] = defensa;
        teclas["salto"] = salto;
        teclas["especial"] = especial;
    }

    sf::Keyboard::Key moverArriba() const { return teclas.at("arriba"); }
    sf::Keyboard::Key moverAbajo() const { return teclas.at("abajo"); }
    sf::Keyboard::Key moverDerecha() const { return teclas.at("derecha"); }
    sf::Keyboard::Key moverIzquierda() const { return teclas.at("izquierda"); }
    sf::Keyboard::Key teclaAtaque() const { return teclas.at("ataque"); }
    sf::Keyboard::Key teclaDefensa() const { return teclas.at("defensa"); }
    sf::Keyboard::Key teclaSalto() const { return teclas.at("salto"); }
    sf::Keyboard::Key teclaEspecial() const { return teclas.at("especial"); }

    void cambiarTecla(const std::string& accion, sf::Keyboard::Key nuevaTecla)
    {
        if (teclas.find(accion) != teclas.end())
        {
            teclas[accion] = nuevaTecla;
        }
    }

    bool estaPresionada(const std::string& accion) const
    {
        auto it = teclas.find(accion);
        if (it != teclas.end())
        {
            return sf::Keyboard::isKeyPressed(it->second);
        }
        return false;
    }

    void mostrarControles() const
    {
        for (const auto& par : teclas)
        {
            std::cout << "Accion: " << par.first << " -> Tecla: " << static_cast<int>(par.second) << std::endl;
        }
    }
};

