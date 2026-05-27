#include <SFML/Graphics.hpp>
#include "Player.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Academic Project - Platform Runner");

    // Karakterimizi oluşturuyoruz (Boyut: 50x50, Pozisyon: x=200, y=400)
    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(200.0f, 400.0f));

    while (window.isOpen()) {
        // SFML 3.0 Modern Event Döngüsü
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 1. Girdileri Kontrol Et ve Güncelle
        player.update();

        // 2. Ekranı Temizle
        window.clear(sf::Color::Black);

        // 3. Objeleri Çiz
        player.draw(window);

        // 4. Ekranda Göster
        window.display();
    }

    return 0;
}