#include "Player.hpp"

Player::Player(sf::Vector2f size, sf::Vector2f position) {
    body.setSize(size);
    body.setFillColor(sf::Color::Cyan);
    body.setPosition(position);
    speed = 4.0f;          // Sağa sola pürüzsüz hareket hızı
    
    velocityY = 0.0f;
    gravity = 0.6f;        // Karakterin pürüzsüz düşmesi için ideal ivme
    jumpSpeed = -14.0f;    // 60 FPS'te belirgin bir zıplama için ideal güç
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

    // --- GARANTİLİ YER ÇEKİMİ VE ZEMİN KONTROLÜ ---
    // Eğer karakter pencerenin alt sınırına (örneğin y = 500) ulaşmadıysa düşsün
    if (body.getPosition().y < 500.0f) {
        velocityY += gravity;
        // Çok küçük bir toleransla havada olup olmadığını esnetiyoruz
        if (body.getPosition().y < 498.0f) {
            isGrounded = false;
        }
    } else {
        // Karakter zemine tam otursun
        body.setPosition(sf::Vector2f(body.getPosition().x, 500.0f));
        velocityY = 0.0f;
        isGrounded = true; // Karakter kesinlikle yerde!
    }

    // --- ZIPLAMA KONTROLÜ ---
    // Karakter kesin olarak yerdeyse Space algılansın
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded) {
        velocityY = jumpSpeed;
        isGrounded = false; // Zıpladığı an yer bağını koparıyoruz
    }

    // Hesaplanan dikey hareketi uyguluyoruz
    body.move(sf::Vector2f(0.0f, velocityY));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(body);
}