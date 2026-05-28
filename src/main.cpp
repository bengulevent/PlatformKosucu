#include <SFML/Graphics.hpp>
#include "Player.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Runner");
    window.setFramerateLimit(60); // 60 FPS Sabitleme

    // Zemin Tanımlaması
    sf::RectangleShape ground(sf::Vector2f(800.0f, 50.0f));
    ground.setFillColor(sf::Color(50, 50, 50));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    // Karakter Tanımlaması
    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(200.0f, 100.0f));

    // --- SENKRONİZE ENGEL TANIMLAMASI ---
    sf::RectangleShape obstacle(sf::Vector2f(40.0f, 40.0f));
    obstacle.setFillColor(sf::Color::Red);
    obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
    float obstacleSpeed = 5.0f;

    // Oyun Döngüsü
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // --- TÜM GÜNCELLEMELER ARKA ARKAYA SENKRONİZE ---
        player.update();

        // Engel Hareketi
        obstacle.move(sf::Vector2f(-obstacleSpeed, 0.0f));
        if (obstacle.getPosition().x < -40.0f) {
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
            obstacleSpeed += 0.2f; // Zorluk artışı
        }

        // --- ÇIZIM AŞAMASI (TAM SENKRON) ---
        window.clear(sf::Color::Black);
        
        window.draw(ground);            // 1. En arka katman zemin
        window.draw(obstacle);          // 2. Engel
        player.draw(window);            // 3. En ön katman karakter
        
        window.display();               // Ekran kartına tek seferde gönder
    }

    return 0;
}