#include "Player.hpp"

Player::Player(const sf::Vector2f& size, const sf::Vector2f& position) {
    if (m_texture.loadFromFile("assets/karakter.jpg")) {
        sprite = new sf::Sprite(m_texture);
        
        // Karakterin ayak tabanının tam orta noktasını merkez (orijin) alıyoruz
        sprite->setOrigin(sf::Vector2f(111.0f, 375.0f));
        
        // Karakter boyutunu sahneye kibar ve dengeli oturtmak için sabit ölçek
        sprite->setScale(sf::Vector2f(0.35f, 0.35f));
    } else {
        sprite = nullptr;
    }

    bounds.size = size;
    bounds.position = position;
}

Player::~Player() {
    if (sprite) {
        delete sprite;
    }
}

void Player::update(bool moveLeft, bool moveRight, bool isFiring) {
    if (sprite) {
        sprite->setPosition(bounds.position);

        if (isFiring) {
            // Ateş etme karesi: Tam genişlik (X: 444, Genişlik: 222)
            sprite->setTextureRect(sf::IntRect(sf::Vector2i(444, 0), sf::Vector2i(222, 375)));
        } 
        else if (moveLeft || moveRight) {
            // KOŞMA DÜZELTMESİ: 
            // Yan kareden silah/ateş sızmaması için X koordinatını 222 yerine 215'e çekiyoruz 
            // ve genişliği 205 piksel olarak daraltıyoruz.
            sprite->setTextureRect(sf::IntRect(sf::Vector2i(215, 0), sf::Vector2i(205, 375)));
            
            if (moveLeft) {
                sprite->setScale(sf::Vector2f(-0.35f, 0.35f)); 
            } else {
                sprite->setScale(sf::Vector2f(0.35f, 0.35f));  
            }
        } 
        else {
            // Sabit durma karesi (X: 0, Genişlik: 215)
            sprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(215, 375)));
        }
    }
}

sf::FloatRect Player::getBounds() const {
    return bounds;
}

void Player::resetPosition(const sf::Vector2f& position) {
    bounds.position = position;
    if (sprite) {
        sprite->setPosition(position);
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (sprite) {
        window.draw(*sprite);
    }
}