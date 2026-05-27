#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(sf::Vector2f size, sf::Vector2f position);
    void update();
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape body;
    float speed;
    
    // Fizik ve Zıplama Değişkenleri
    float velocityY;
    float gravity;
    bool isGrounded;
    float jumpSpeed; 
};

#endif