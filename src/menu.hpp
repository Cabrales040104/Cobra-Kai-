#include <SFML/Graphics.hpp>

const int X_size = 800;
const int Y_size = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(X_size, Y_size), "Cobra Kai Menu");
    
    while (window.isOpen()) {
        sf::Event event;
        sf::text titulo("Cobra Kai", sf::Font(), 50);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}   
 

