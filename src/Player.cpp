#include "Player.hpp"

Player::Player(sf::Vector2f size, sf::Vector2f position) {
    body.setSize(size);
    body.setFillColor(sf::Color::Cyan);
    body.setPosition(position);
    speed = 4.0f;          // Yatay hız
    
    velocityY = 0.0f;
    gravity = 0.6f;        // Yer çekimi
    jumpSpeed = -16.0f;    // Zıplama gücü
    isGrounded = false;
}

void Player::update() {
    // Tüm hareketleri bu x ve y değerlerinde toplayacağız
    float moveX = 0.0f;

    // Yatay Hareket Kontrolleri
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        moveX = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        moveX = speed;
    }

    // Zıplama Kontrolü
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded) {
        velocityY = jumpSpeed;
        isGrounded = false;
    }

    // Yer çekimini dikey hıza sürekli ekle
    velocityY += gravity;

    // Karakteri TEK BİR SEFERDE hem X hem Y ekseninde hareket ettiriyoruz
    body.move(sf::Vector2f(moveX, velocityY));

    // --- ZEMİN ÇARPIŞMA KONTROLÜ (Kilit Nokta) ---
    // Eğer karakter yerin altına girdiyse anında zemine sabitle
    if (body.getPosition().y >= 500.0f) {
        body.setPosition(sf::Vector2f(body.getPosition().x, 500.0f));
        velocityY = 0.0f;
        isGrounded = true;
    } else {
        isGrounded = false;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(body);
}