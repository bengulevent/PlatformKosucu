#include "Player.hpp"
#include <iostream>

Player::Player(const sf::Vector2f& size, const sf::Vector2f& position) {
    if (!texture.loadFromFile("assets/karakter.jpg")) {
        std::cout << "HATA: assets/karakter.jpg yuklenemedi!" << std::endl;
    }

    sprite = new sf::Sprite(texture);
    
    // Orijinal resim boyutları
    frameWidth = texture.getSize().x / 3.0f;
    frameHeight = texture.getSize().y;

    currentFrame = 0; 
    isFacingLeft = false;

    frameRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));
    sprite->setTextureRect(frameRect);

    // En-boy oranına göre ölçekleme
    float targetWidth = size.x;   
    float targetHeight = size.y;  

    float scaleX = targetWidth / frameWidth;
    float scaleY = targetHeight / frameHeight;
    sprite->setScale(sf::Vector2f(scaleX, scaleY));

    sprite->setPosition(position);
    bounds = sf::FloatRect(position, sf::Vector2f(targetWidth, targetHeight));
}

Player::~Player() {
    delete sprite;
}

void Player::update(bool moveLeft, bool moveRight, bool firePressed) {
    if (firePressed) {
        currentFrame = 2; 
    } else if (moveLeft || moveRight) {
        currentFrame = 1; 
    } else {
        currentFrame = 0; 
    }

    if (moveLeft) {
        isFacingLeft = true;
    } else if (moveRight) {
        isFacingLeft = false;
    }

    // --- KESİN ÇÖZÜM: TAŞMA PAYI İYİCE ARTIRILDI ---
    // Yan karedeki efektler kesinlikle gözükmesin diye genişliği 35 piksel daraltıyoruz
    int rectX = currentFrame * frameWidth;
    int safeWidth = frameWidth - 35; 
    
    if (isFacingLeft) {
        frameRect = sf::IntRect(sf::Vector2i(rectX + safeWidth, 0), sf::Vector2i(-safeWidth, frameHeight));
    } else {
        frameRect = sf::IntRect(sf::Vector2i(rectX, 0), sf::Vector2i(safeWidth, frameHeight));
    }
    
    sprite->setTextureRect(frameRect);
    bounds.position = sprite->getPosition();
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite); 
}

sf::FloatRect Player::getBounds() const {
    return bounds;
}

void Player::resetPosition(const sf::Vector2f& newPos) {
    sprite->setPosition(newPos);
    bounds.position = newPos; 
}