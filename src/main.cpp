#include "Juego.hpp"
#include <iostream>

int main()
{
    try
    {
        Juego juego;
        juego.iniciar();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}