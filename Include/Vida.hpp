#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Vida
{
private:
  int maxHealth;
  int currentHealth;
  sf::Font font;
  sf::Text healthText;
  sf::Vector2f position;

public:
  Vida(int maxHealth, sf::Vector2f position)
      : maxHealth(maxHealth), currentHealth(maxHealth), position(position)
  {
    if (!font.loadFromFile("./assets/fonts/Minecraft.ttf"))
    {
      throw std::runtime_error("No se pudo cargar la fuente");
    }
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::Green);
    healthText.setPosition(position);
    healthText.setString(std::to_string(currentHealth) + "/" + std::to_string(maxHealth));
  }

  void takeDamage(int damage)
  {
    currentHealth -= damage;
    if (currentHealth < 0)
      currentHealth = 0;
    healthText.setString(std::to_string(currentHealth) + "/" + std::to_string(maxHealth));
  }

  void dibujar(sf::RenderWindow &window)
  {
    sf::RectangleShape healthBar(sf::Vector2f(static_cast<float>(currentHealth) / maxHealth * 100, 10));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(position);

    window.draw(healthBar);
    window.draw(healthText);
  }

  int getCurrentHealth() const { return currentHealth; }
};