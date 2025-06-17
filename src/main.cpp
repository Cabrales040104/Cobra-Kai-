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

    // Crear una lista de imágenes que se mostrarán en el menú
    std::vector<std::string> imagenes = {"1.png"};
        
    Menu menu(imagenes); // Crear el menú con las imágenes
    menu.ajustarImagenAITamanoVentana(window.getSize());
    Campo campo("1.png");

    // Controladores y personajes como antes
    Control control1;
    Control control2(
        sf::Keyboard::W,     // arriba
        sf::Keyboard::S,     // abajo
        sf::Keyboard::D,     // derecha
        sf::Keyboard::A,     // izquierda
        sf::Keyboard::J,     // ataque
        sf::Keyboard::K,     // defensa
        sf::Keyboard::L,     // salto
        sf::Keyboard::U      // especial
    );
   const int alturaVentana = 654; // Altura de la ventana del juego
    const float posYPersonaje = alturaVentana - 10; // Posición Y del personaje (10 px sobre el suelo)

    Personaje Dan(sf::Vector2f(610, posYPersonaje), "ADann.png", control1, {601, 55});
    Personaje Johnn(sf::Vector2f(260, posYPersonaje), "AJohnn.png", control2, {8, 55});

    // Crear el texto de victoria
    sf::Text winText;
    winText.setFont(menu.getFont());        // Usamos el método público para acceder a la fuente
    winText.setCharacterSize(30);           // Tamaño del texto
    winText.setFillColor(sf::Color::White); // Color blanco para el texto

    bool gameOver = false;   // Para controlar si el juego terminó
    std::string winner = ""; // Para almacenar quién ganó

    // ----------- CICLO DEL MENÚ DE INICIO -----------
    bool enMenu = true;
    while (window.isOpen() && enMenu)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                enMenu = false; // Salir del menú si se presiona Enter
        }

        menu.actualizar(); // Actualizar el menú (para cambiar las imágenes)
        window.clear();
        menu.dibujar(window); // Dibujar el menú (que incluye la imagen y el texto)
        window.display();
    }
    // ----------- FIN DEL CICLO DEL MENÚ DE INICIO -----------

    campo.cambiarImagen("5.png", window.getSize()); // Cambiar el fondo del campo de batalla

    // Aquí ya comenzaría el juego con los personajes
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        Dan.leerTeclado(sf::Keyboard::K);
        Johnn.leerTeclado(sf::Keyboard::F);

        Dan.actualizar();
        Johnn.actualizar();

        Colision::manejar(Dan, Johnn);

        // Comprobar si alguien ha ganado
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
            Dan.dibujar(window);
            Johnn.dibujar(window);
        }

        window.display();
    }

    return 0;
}