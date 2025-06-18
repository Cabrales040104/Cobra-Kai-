#pragma once
#include <SFML/Graphics.hpp>
#include "Vida.hpp"

class Vida {
private:
    int maxHealth;
    int currentHealth;
    sf::Vector2f position;
    sf::Vector2f size;
    sf::RectangleShape background;
    sf::RectangleShape bar;

public:
    Vida(int maxHealth, sf::Vector2f position, sf::Vector2f size = sf::Vector2f(150, 20))
        : maxHealth(maxHealth), currentHealth(maxHealth), position(position), size(size)
    {
        background.setSize(size);
        background.setFillColor(sf::Color(50, 50, 50));
        background.setPosition(position);

        bar.setSize(size);
        bar.setFillColor(sf::Color::Green);
        bar.setPosition(position);
    }

    void takeDamage(int damage) {
        currentHealth -= damage;
        if (currentHealth < 0) currentHealth = 0;
        updateBar();
    }

    void heal(int amount) {
        currentHealth += amount;
        if (currentHealth > maxHealth) currentHealth = maxHealth;
        updateBar();
    }

    int getCurrentHealth() const {
        return currentHealth;
    }

    int getMaxHealth() const {
        return maxHealth;
    }

    void dibujar(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(bar);
    }

private:
    void updateBar() {
        float healthPercent = static_cast<float>(currentHealth) / maxHealth;
        bar.setSize(sf::Vector2f(size.x * healthPercent, size.y));
        if (healthPercent > 0.5f)
            bar.setFillColor(sf::Color::Green);
        else if (healthPercent > 0.2f)
            bar.setFillColor(sf::Color::Yellow);
        else
            bar.setFillColor(sf::Color::Red);
    }
};