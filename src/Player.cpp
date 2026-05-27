#include "Player.hpp"

Player::Player(sf::Vector2f size, sf::Vector2f position) {
    body.setSize(size);
    body.setFillColor(sf::Color::Cyan);
    body.setPosition(position);
    speed = 4.0f;          // Sağa sola hareket hızı
    
    velocityY = 0.0f;
    gravity = 0.4f;        // Yer çekimi ivmesi
    jumpSpeed = -12.0f;    // Zıplama gücü
    isGrounded = false;
}

void Player::update() {
    // Sol ve Sağ Hareket
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        body.move(sf::Vector2f(-speed, 0.0f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        body.move(sf::Vector2f(speed, 0.0f));
    }

    // Zıplama Kontrolü
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded) {
        velocityY = jumpSpeed;
        isGrounded = false;
    }

    // Yer Çekimi Fiziği
    if (body.getPosition().y < 450.0f) {
        velocityY += gravity;
        isGrounded = false;
    } else {
        body.setPosition(sf::Vector2f(body.getPosition().x, 450.0f));
        velocityY = 0.0f;
        isGrounded = true;
    }

    // Dikey hareketi uygula
    body.move(sf::Vector2f(0.0f, velocityY));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(body);
}