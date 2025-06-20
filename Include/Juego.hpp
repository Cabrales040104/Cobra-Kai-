#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Campo.hpp"
#include "Colision.hpp"
#include "Control.hpp"
#include "Personaje.hpp"
#include "Menu.hpp"
#include "Vida.hpp"
#include <string>
#include <iostream>
#include <memory>

class Juego
{
private:
    // Ventana y configuración
    sf::RenderWindow window;
    const int ANCHO_VENTANA = 960;
    const int ALTO_VENTANA = 654;
    
    // Audio
    sf::Music musicaFondo;
    
    // Componentes del juego
    std::unique_ptr<Menu> menu;
    std::unique_ptr<Campo> campo;
    std::unique_ptr<Personaje> jugador1;
    std::unique_ptr<Personaje> jugador2;
    
    // Controles
    Control control1;
    Control control2;
    
    // Estado del juego
    enum EstadoJuego { MENU, JUGANDO, GAME_OVER };
    EstadoJuego estadoActual;
    
    // Texto de victoria
    sf::Text textoVictoria;
    sf::Clock clockGameOver;
    bool juegoTerminado;
    std::string ganador;

public:
    Juego() : estadoActual(MENU), juegoTerminado(false), ganador("")
    {
        inicializarVentana();
        inicializarAudio();
        inicializarControles();
        inicializarComponentes();
        inicializarTextos();
    }

    ~Juego()
    {
        if (musicaFondo.getStatus() == sf::Music::Playing)
        {
            musicaFondo.stop();
        }
    }

    void iniciar()
    {
        while (window.isOpen())
        {
            manejarEventos();
            actualizar();
            renderizar();
        }
    }

private:
    void inicializarVentana()
    {
        window.create(sf::VideoMode(ANCHO_VENTANA, ALTO_VENTANA), "Cobra Kai", 
                      sf::Style::Close | sf::Style::Titlebar);
        window.setFramerateLimit(60);
    }

    void inicializarAudio()
    {
        if (!musicaFondo.openFromFile("assets/sounds/AudioRocky.mp3"))
        {
            throw std::runtime_error("No se pudo cargar la música de fondo");
        }
        musicaFondo.setLoop(true);
        musicaFondo.play();
    }

    void inicializarControles()
    {
        control1 = Control(
            sf::Keyboard::Right, // derecha
            sf::Keyboard::Left,  // izquierda
            sf::Keyboard::N,     // ataque
            sf::Keyboard::M      // defensa
        );
        
        control2 = Control(
            sf::Keyboard::D, // derecha
            sf::Keyboard::A, // izquierda
            sf::Keyboard::F, // ataque
            sf::Keyboard::G  // defensa
        );
    }

    void inicializarComponentes()
    {
        // Inicializar menú
        std::vector<std::string> imagenes = {"1.png"};
        menu = std::make_unique<Menu>(imagenes);
        menu->ajustarImagenAITamanoVentana(window.getSize());
        
        // Inicializar campo
        campo = std::make_unique<Campo>("5.png");
        campo->cambiarImagen("5.png", window.getSize());
        
        // Inicializar personajes
        const float posYPersonaje = ALTO_VENTANA - 10;
        jugador1 = std::make_unique<Personaje>(
            sf::Vector2f(610, posYPersonaje), 
            "ADann.png", 
            control1, 
            sf::Vector2f(601, 55)
        );
        
        jugador2 = std::make_unique<Personaje>(
            sf::Vector2f(260, posYPersonaje), 
            "AJohnn.png", 
            control2, 
            sf::Vector2f(8, 55)
        );
    }

    void inicializarTextos()
    {
        textoVictoria.setFont(menu->getFont());
        textoVictoria.setCharacterSize(40);
        textoVictoria.setFillColor(sf::Color::White);
        textoVictoria.setStyle(sf::Text::Bold);
    }

    void manejarEventos()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            
            switch (estadoActual)
            {
                case MENU:
                    if (event.type == sf::Event::KeyPressed && 
                        event.key.code == sf::Keyboard::Enter)
                    {
                        estadoActual = JUGANDO;
                    }
                    break;
                    
                case GAME_OVER:
                    if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::Enter)
                        {
                            reiniciarJuego();
                        }
                        else if (event.key.code == sf::Keyboard::Escape)
                        {
                            window.close();
                        }
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }

    void actualizar()
    {
        switch (estadoActual)
        {
            case MENU:
                actualizarMenu();
                break;
            case JUGANDO:
                actualizarJuego();
                break;
            case GAME_OVER:
                actualizarGameOver();
                break;
        }
    }

    void actualizarMenu()
    {
        menu->actualizar();
    }

    void actualizarJuego()
    {
        // Leer controles de los jugadores
        jugador1->leerTeclado(sf::Keyboard::N);
        jugador2->leerTeclado(sf::Keyboard::F);
        
        // Actualizar personajes
        jugador1->actualizar();
        jugador2->actualizar();
        
        // Manejar colisiones
        Colision::manejar(*jugador1, *jugador2);
        
        // Verificar condiciones de victoria
        verificarVictoria();
    }

    void actualizarGameOver()
    {
        // Esperar un tiempo antes de permitir acciones
        if (clockGameOver.getElapsedTime().asSeconds() < 1.0f)
        {
            return;
        }
    }

    void verificarVictoria()
    {
        if (jugador1->getHealth() <= 0)
        {
            ganador = "¡Ganó Jugador 2 (WASD + F/G)!";
            estadoActual = GAME_OVER;
            clockGameOver.restart();
        }
        else if (jugador2->getHealth() <= 0)
        {
            ganador = "¡Ganó Jugador 1 (Flechas + N/M)!";
            estadoActual = GAME_OVER;
            clockGameOver.restart();
        }
    }

    void renderizar()
    {
        window.clear();
        
        switch (estadoActual)
        {
            case MENU:
                renderizarMenu();
                break;
            case JUGANDO:
                renderizarJuego();
                break;
            case GAME_OVER:
                renderizarGameOver();
                break;
        }
        
        window.display();
    }

    void renderizarMenu()
    {
        menu->dibujar(window);
    }

    void renderizarJuego()
    {
        campo->dibujar(window);
        jugador1->dibujar(window);
        jugador2->dibujar(window);
    }

    void renderizarGameOver()
    {
        // Mostrar el campo de fondo
        campo->dibujar(window);
        
        // Configurar y mostrar texto de victoria
        textoVictoria.setString(ganador + "\n\nPresiona ENTER para jugar de nuevo\nPresiona ESC para salir");
        
        // Centrar el texto
        sf::FloatRect textBounds = textoVictoria.getLocalBounds();
        textoVictoria.setPosition(
            (ANCHO_VENTANA - textBounds.width) / 2,
            (ALTO_VENTANA - textBounds.height) / 2
        );
        
        // Dibujar un fondo semi-transparente para el texto
        sf::RectangleShape fondo(sf::Vector2f(textBounds.width + 40, textBounds.height + 40));
        fondo.setFillColor(sf::Color(0, 0, 0, 180));
        fondo.setPosition(
            textoVictoria.getPosition().x - 20,
            textoVictoria.getPosition().y - 20
        );
        
        window.draw(fondo);
        window.draw(textoVictoria);
    }

    void reiniciarJuego()
    {
        // Reinicializar personajes
        const float posYPersonaje = ALTO_VENTANA - 10;
        
        jugador1 = std::make_unique<Personaje>(
            sf::Vector2f(610, posYPersonaje), 
            "ADann.png", 
            control1, 
            sf::Vector2f(601, 55)
        );
        
        jugador2 = std::make_unique<Personaje>(
            sf::Vector2f(260, posYPersonaje), 
            "AJohnn.png", 
            control2, 
            sf::Vector2f(8, 55)
        );
        
        // Resetear estado
        juegoTerminado = false;
        ganador = "";
        estadoActual = JUGANDO;
    }
};