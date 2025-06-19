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
    sf::RenderWindow window(sf::VideoMode(960, 654), "Cobra Kai", sf::Style::Close | sf::Style::Titlebar);

    sf::Music musicaFondo;
    if (!musicaFondo.openFromFile("assets/sounds/AudioRocky.mp3"))
    {
        throw std::runtime_error("No se pudo cargar la música de fondo");
    }
    musicaFondo.setLoop(true);
    musicaFondo.play();

    std::vector<std::string> imagenes = {"1.png"};

    Menu menu(imagenes);
    menu.ajustarImagenAITamanoVentana(window.getSize());
    Campo campo("1.png");

    Control control1(
        sf::Keyboard::Right, // derecha
        sf::Keyboard::Left,  // izquierda
        sf::Keyboard::N,     // ataque
        sf::Keyboard::M      // defensa
    );
    Control control2(
        sf::Keyboard::D, // derecha
        sf::Keyboard::A, // izquierda
        sf::Keyboard::F, // ataque
        sf::Keyboard::G  // defensa
    );
    const int alturaVentana = 654;
    const float posYPersonaje = alturaVentana - 10;

    Personaje Dan(sf::Vector2f(610, posYPersonaje), "ADann.png", control1, {601, 55});
    Personaje Johnn(sf::Vector2f(260, posYPersonaje), "AJohnn.png", control2, {8, 55});

    sf::Text winText;
    winText.setFont(menu.getFont());
    winText.setCharacterSize(30);
    winText.setFillColor(sf::Color::White);

    bool gameOver = false;
    std::string winner = "";

    bool enMenu = true;
    while (window.isOpen() && enMenu)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                enMenu = false;
        }

        menu.actualizar();
        window.clear();
        menu.dibujar(window);
        window.display();
    }

    campo.cambiarImagen("5.png", window.getSize());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        Dan.leerTeclado(sf::Keyboard::N);
        Johnn.leerTeclado(sf::Keyboard::F);

        Dan.actualizar();
        Johnn.actualizar();

        Colision::manejar(Dan, Johnn);

        if (Dan.getHealth() == 0)
        {
            winner = "¡Gano Jugador 1!";
            gameOver = true;
        }
        else if (Johnn.getHealth() == 0)
        {
            winner = "¡Gano jugador 2!";
            gameOver = true;
        }

        window.clear();

        if (gameOver)
        {
            winText.setString(winner);                                                                                                                      // Establecer el texto del ganador
            winText.setPosition(window.getSize().x / 2 - winText.getLocalBounds().width / 2, window.getSize().y / 2 - winText.getLocalBounds().height / 2); // Centrar el texto
            window.draw(winText);                                                                                                                           // Dibujar el texto de victoria
            window.display();                                                                                                                               // Mostrar el mensaje de victoria

            sf::sleep(sf::seconds(2));

            break;
        }
        else
        {

            campo.dibujar(window);
            Dan.dibujar(window);
            Johnn.dibujar(window);
        }

        window.display();
    }

    return 0;
}