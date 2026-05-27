#include "Player.hpp"

// Constructor: Karakterin boyutunu, rengini ve başlangıç pozisyonunu ayarlıyoruz
Player::Player(sf::Vector2f size, sf::Vector2f position) {
    body.setSize(size);
    body.setFillColor(sf::Color::Cyan); // Siyah ekranda güzel parlar :)
    body.setPosition(position);
    speed = 0.5f; // Hareket hızı
}

void Player::update() {
    // SFML 3.0 Klavye Kontrolü (Key:: eklenmesi gerekiyor)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        body.move(sf::Vector2f(-speed, 0.0f));
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        body.move(sf::Vector2f(speed, 0.0f));
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(body);
}