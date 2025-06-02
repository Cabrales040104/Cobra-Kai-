#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Campo.hpp>
#include <Colision.hpp>
#include <Control.hpp>
#include <Personaje.hpp>
#include <Menu.hpp>
#include <Vida.hpp>
#include <string>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 435), "Cobra Kai", sf::Style::Close | sf::Style::Titlebar);

    // Crear una lista de imágenes que se mostrarán en el menú
    std::vector<std::string> imagenes = {"9.png", "1.png", "14.png", "2.png", "26.png", "10.png", "3.png"};

    Menu menu(imagenes); // Crear el menú con las imágenes
    Campo campo("1.png");

    // Controladores y personajes como antes
    Control control1;
    Control control2(sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::D, sf::Keyboard::A);

    Personaje ken(sf::Vector2f(440, 240), "pikachu.png", control1, {381, 55});
    Personaje pika(sf::Vector2f(130, 240), "pikachu.png", control2, {8, 55});

    // Crear el texto de victoria
    sf::Text winText;
    winText.setFont(menu.getFont());        // Usamos el método público para acceder a la fuente
    winText.setCharacterSize(30);           // Tamaño del texto
    winText.setFillColor(sf::Color::White); // Color blanco para el texto

    bool gameOver = false;   // Para controlar si el juego terminó
    std::string winner = ""; // Para almacenar quién ganó

    // Ciclo para mostrar el menú
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Actualizar el menú (para cambiar las imágenes)
        menu.actualizar();

        // Si se presiona Enter, iniciar el juego
        if (menu.iniciarJuego())
        {
            break; // Salir del ciclo de menú y comenzar el juego
        }

        window.clear();
        menu.dibujar(window); // Dibujar el menú (que incluye la imagen y el texto)
        window.display();
    }

    // Aquí ya comenzaría el juego con los personajes
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ken.leerTeclado(sf::Keyboard::K);
        pika.leerTeclado(sf::Keyboard::F);

        ken.actualizar();
        pika.actualizar();

        Colision::manejar(ken, pika);

        // Comprobar si alguien ha ganado
        if (ken.getHealth() == 0)
        {
            winner = "¡Gano Jugador 1!";
            gameOver = true;
        }
        else if (pika.getHealth() == 0)
        {
            winner = "¡Gano jugador 2!";
            gameOver = true;
        }

        window.clear();

        // Si el juego ha terminado, mostrar el mensaje de victoria
        if (gameOver)
        {
            winText.setString(winner);                                                                                                                      // Establecer el texto del ganador
            winText.setPosition(window.getSize().x / 2 - winText.getLocalBounds().width / 2, window.getSize().y / 2 - winText.getLocalBounds().height / 2); // Centrar el texto
            window.draw(winText);                                                                                                                           // Dibujar el texto de victoria
            window.display();                                                                                                                               // Mostrar el mensaje de victoria

            // Esperar un momento antes de cerrar el juego
            sf::sleep(sf::seconds(2)); // Esperar 2 segundos para que el jugador vea el mensaje de victoria

            break; // Salir del ciclo para cerrar el juego después de la victoria
        }
        else
        {
            // Si el juego no ha terminado, se sigue actualizando y dibujando los personajes
            campo.dibujar(window);
            ken.dibujar(window);
            pika.dibujar(window);
        }

        window.display();
    }

    return 0;
};