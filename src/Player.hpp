#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const sf::Vector2f& size, const sf::Vector2f& position);
    ~Player();

    void update(bool moveLeft, bool moveRight, bool isFiring);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void resetPosition(const sf::Vector2f& position);

private:
    sf::FloatRect bounds;
    sf::Sprite* sprite;
    sf::Texture m_texture;
};

#endif