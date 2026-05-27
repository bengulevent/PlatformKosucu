#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
private:
    sf::RectangleShape body;
    float speed;
    float velocityY;    // Dikey düşme hızı
float gravity;      // Yer çekimi ivmesi
bool isGrounded;    // Yerde mi kontrolü

public:
    Player(sf::Vector2f size, sf::Vector2f position);
    
    // Karakterin hareket mantığını burada işleyeceğiz
    void update(); 
    
    // Karakteri ekrana çizmek için kullanacağız
    void draw(sf::RenderWindow& window); 
};

#endif