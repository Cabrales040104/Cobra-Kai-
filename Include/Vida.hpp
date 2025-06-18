// Lógica de ataque y defensa para ambos personajes
static sf::Clock relojGolpe;
float tiempoEntreGolpes = 0.5f; // medio segundo entre golpes
float tiempo = relojGolpe.getElapsedTime().asSeconds();
int daño = 0.1;

// Jugador 1 ataca
if (control1.estaPresionada("jugador1", "ataque") && tiempo >= tiempoEntreGolpes)
{
    if (!control2.estaPresionada("jugador2", "defensa"))
    {
        Johnn.restarVida(daño); // le quita 10 de vida
        std::cout << "Jugador 1 golpea → Vida jugador 2: " << Johnn.getHealth() << std::endl;
    }
    else
    {
        std::cout << "Jugador 2 se defendió del ataque de Jugador 1" << std::endl;
    }
    relojGolpe.restart();
}

// Jugador 2 ataca
if (control2.estaPresionada("jugador2", "ataque") && tiempo >= tiempoEntreGolpes)
{
    if (!control1.estaPresionada("jugador1", "defensa"))
    {
        Dan.restarVida(daño); // le quita 10 de vida
        std::cout << "Jugador 2 golpea → Vida jugador 1: " << Dan.getHealth() << std::endl;
    }
    else
    {
        std::cout << "Jugador 1 se defendió del ataque de Jugador 2" << std::endl;
    }
    relojGolpe.restart();
}