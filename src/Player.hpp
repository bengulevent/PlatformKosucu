#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    // Yapıcı ve Temel Fonksiyonlar (Sadece birer kez tanımlı)
    Player(sf::Vector2f size, sf::Vector2f position);
    void update();
    void draw(sf::RenderWindow& window);
    
    // Çarpışma ve Reset Fonksiyonları
    sf::FloatRect getBounds() const { return body.getGlobalBounds(); }
    void resetPosition(sf::Vector2f newPos) { body.setPosition(newPos); velocityY = 0.0f; }

private:
    sf::RectangleShape body;
    float speed;
    float velocityY;
    float gravity;
    float jumpSpeed;
    bool isGrounded;
};

#endif