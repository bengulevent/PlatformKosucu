#include <SFML/Graphics.hpp>
#include "Player.hpp"

int main() {
    // 800x600 boyutunda oyun penceremizi açıyoruz
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Runner");
    
    // --- OYUNU 60 FPS'E SABİTLİYORUZ (HIZ PROBLEMİNİ ÇÖZEN SATIR) ---
    window.setFramerateLimit(60);

    // Karakterimizi 50x50 boyutuyla, ekranın ortasında (200, 100) başlatıyoruz
    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(200.0f, 100.0f));

    // Oyun döngüsü
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Güncelleme (Hareket ve Fizik)
        player.update();

        // Çizim Aşaması
        window.clear(sf::Color::Black); // Arka plan siyah
        player.draw(window);            // Karakteri çiz
        window.display();               // Ekrana yansıt
    }

    return 0;
}