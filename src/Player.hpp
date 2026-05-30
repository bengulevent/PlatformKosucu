#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite* sprite;      // SFML 3.0 kurucu hatasını önleyen pointer
    sf::Texture texture;     // Sınıf ömrü boyunca yok olmaması için burada kalmalı
    sf::FloatRect bounds;

    sf::IntRect frameRect;   
    int currentFrame;        
    bool isFacingLeft;       
    float frameWidth;        
    float frameHeight;       

public:
    Player(const sf::Vector2f& size, const sf::Vector2f& position);
    ~Player(); 
    
    void update(bool moveLeft, bool moveRight, bool firePressed);
    void draw(sf::RenderWindow& window);
    
    sf::FloatRect getBounds() const;
    void resetPosition(const sf::Vector2f& newPos);
};

#endif