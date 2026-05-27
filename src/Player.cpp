#include "Player.hpp"

// Constructor: Karakterin boyutunu, rengini ve başlangıç pozisyonunu ayarlıyoruz
Player::Player(sf::Vector2f size, sf::Vector2f position) {
    body.setSize(size);
    body.setFillColor(sf::Color::Cyan); // Siyah ekranda güzel parlar :)
    body.setPosition(position);
    speed = 0.5f; // Hareket hızı
    velocityY = 0.0f;
gravity = 0.002f;
isGrounded = false;
}

void Player::update() {
    // SFML 3.0 Klavye Kontrolü (Key:: eklenmesi gerekiyor)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        body.move(sf::Vector2f(-speed, 0.0f));
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        body.move(sf::Vector2f(speed, 0.0f));
    }
    if (body.getPosition().y < 450.0f) {
    velocityY += gravity;
    isGrounded = false;
} else {
    body.setPosition(sf::Vector2f(body.getPosition().x, 450.0f));
    velocityY = 0.0f;
    isGrounded = true;
}
body.move(sf::Vector2f(0.0f, velocityY));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(body);
}