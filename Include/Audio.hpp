#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class Sonido
{
private:
    sf::Music musica;
    std::map<std::string, sf::SoundBuffer> buffers;
    std::map<std::string, sf::Sound> sonidos;

public:
    void cargarMusica(const std::string &ruta);
    void playMusica(bool loop = true);

    void cargarSonido(const std::string &nombre, const std::string &ruta);
    void playSonido(const std::string &nombre);
};