#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Runner");
    window.setFramerateLimit(60); 

    // Önce Font Yüklemesi
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        return -1; 
    }

    // Yazı Objesi Tanımlaması
    sf::Text scoreText(font);
    scoreText.setCharacterSize(40);            // Yazıyı iyice büyüttüm rahat gör diye kanki
    scoreText.setFillColor(sf::Color::White);  
    scoreText.setPosition(sf::Vector2f(30.0f, 30.0f)); 

    // Zemin ve Karakter
    sf::RectangleShape ground(sf::Vector2f(800.0f, 50.0f));
    ground.setFillColor(sf::Color(50, 50, 50));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(200.0f, 100.0f));

    // Engel
    sf::RectangleShape obstacle(sf::Vector2f(40.0f, 40.0f));
    obstacle.setFillColor(sf::Color::Red);
    obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
    float obstacleSpeed = 5.0f;

    // Sadece temiz bir skor değişkeni bırakıyoruz
    int score = 0;

    // Oyun Döngüsü
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        player.update();

        // Engel Hareketi (Her karede sola kaysın)
        obstacle.move(sf::Vector2f(-obstacleSpeed, 0.0f));

       
        // Kırmızı kutu sol sınırdan tamamen çıktığı mikrosaniyede:
        if (obstacle.getPosition().x < -40.0f) {
            score++; // Skoru tertemiz 1 artırıyoruz (Çakışma ihtimali sıfır)
            
            // Engeli sağ tarafa geri ışınla
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
            obstacleSpeed += 0.3f; // Oyun her skorda biraz hızlansın
        }

        // Çarpışma Kontrolü
        sf::FloatRect playerBounds = player.getBounds();
        sf::FloatRect obstacleBounds = obstacle.getGlobalBounds();

        if (playerBounds.findIntersection(obstacleBounds).has_value()) {
            player.resetPosition(sf::Vector2f(200.0f, 100.0f)); 
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f)); 
            obstacleSpeed = 5.0f; 
            score = 0; // Yanınca skoru sıfırla
        }

        // Metni hafızada güncelle
        scoreText.setString("Skor: " + std::to_string(score));

        // Çizim Aşaması
        window.clear(sf::Color::Black);
        
        window.draw(ground);            
        window.draw(obstacle);          
        player.draw(window);            
        window.draw(scoreText); // Skoru çiz
        
        window.display();               
    }

    return 0;
}