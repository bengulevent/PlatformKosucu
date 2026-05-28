#include <SFML/Graphics.hpp>
#include "Player.hpp"

int main() {
    // 800x600 boyutunda oyun penceremizi açıyoruz
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().bitsPerPixel == 32 ? sf::Vector2u(800, 600) : sf::Vector2u(800, 600)), "Platform Runner");
    
    // Oyun hızını 60 FPS'e sabitliyoruz
    window.setFramerateLimit(60);

    // Karakter ve Zemin objeleri
    sf::RectangleShape ground(sf::Vector2f(800.0f, 50.0f));
    ground.setFillColor(sf::Color(50, 50, 50)); // Koyu Gri
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(200.0f, 100.0f));

    // Oyun döngüsü
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Güncelleme (Fizik ve Hareket)
        player.update();

        // --- ÇİZİM AŞAMASI (Buraya Dikkat Kanki) ---
        window.clear(sf::Color::Black); // Her karede ekranı siyaha boyayıp temizler (İz kalmasını engeller)
        
        window.draw(ground);            // Önce zemini çiz
        player.draw(window);            // Sonra üstüne karakteri çiz
        window.display();               // Hepsini ekrana yansıt
    }

    return 0;
}