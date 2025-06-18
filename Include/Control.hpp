#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class Control {
public:
    sf::Keyboard::Key arriba, abajo, derecha, izquierda, ataque, defensa;

    Control(
        sf::Keyboard::Key arriba,
        sf::Keyboard::Key abajo,
        sf::Keyboard::Key derecha,
        sf::Keyboard::Key izquierda,
        sf::Keyboard::Key ataque,
        sf::Keyboard::Key defensa
    ) : arriba(arriba), abajo(abajo), derecha(derecha), izquierda(izquierda), ataque(ataque), defensa(defensa) {}

    Control() = default;
};

    std::map<std::string, ControlesJugador> jugadores;

public:
    Control() {
        // Controles por defecto para jugador1
        jugadores["jugador1"].teclas = {
            {"arriba", sf::Keyboard::Up},
            {"abajo", sf::Keyboard::Down},
            {"derecha", sf::Keyboard::Right},
            {"izquierda", sf::Keyboard::Left},
            {"ataque", sf::Keyboard::N}, // Cambiado a Shift derecho
            {"defensa", sf::Keyboard::M},

        };

        // Controles por defecto para jugador2
        jugadores["jugador2"].teclas = {
            {"arriba", sf::Keyboard::W},
            {"abajo", sf::Keyboard::S},
            {"derecha", sf::Keyboard::D},
            {"izquierda", sf::Keyboard::A},
            {"ataque", sf::Keyboard::F},
            {"defensa", sf::Keyboard::G},

        };
    }

    // Cambiar una tecla de cierto jugador
    void cambiarTecla(const std::string& jugador, const std::string& accion, sf::Keyboard::Key nuevaTecla)
    {
        if (jugadores.find(jugador) != jugadores.end() &&
            jugadores[jugador].teclas.find(accion) != jugadores[jugador].teclas.end())
        {
            jugadores[jugador].teclas[accion] = nuevaTecla;
        }
    }

    // Verifica si una tecla está presionada para cierto jugador
    bool estaPresionada(const std::string& jugador, const std::string& accion) const
    {
        auto itJugador = jugadores.find(jugador);
        if (itJugador != jugadores.end()) {
            auto itTecla = itJugador->second.teclas.find(accion);
            if (itTecla != itJugador->second.teclas.end()) {
                return sf::Keyboard::isKeyPressed(itTecla->second);
            }
        }
        return false;
    }

    // Obtener la tecla asignada para una acción específica
    sf::Keyboard::Key obtenerTecla(const std::string& jugador, const std::string& accion) const
    {
        return jugadores.at(jugador).teclas.at(accion);
    }

    // Mostrar todas las teclas de ambos jugadores
    void mostrarControles() const
    {
        for (const auto& [nombreJugador, controles] : jugadores)
        {
            std::cout << "Controles de " << nombreJugador << ":\n";
            for (const auto& [accion, tecla] : controles.teclas)
            {
                std::cout << "  " << accion << " -> Tecla: " << static_cast<int>(tecla) << std::endl;
            }
        }
    }
};

#endif // CONTROL_HPP




