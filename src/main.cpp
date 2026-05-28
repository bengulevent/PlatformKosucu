#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Runner");
    window.setFramerateLimit(60); 

    // Font Yüklemesi
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        return -1; 
    }

    // Skor Yazısı
    sf::Text scoreText(font);
    scoreText.setCharacterSize(30);            
    scoreText.setFillColor(sf::Color::White);  
    scoreText.setPosition(sf::Vector2f(20.0f, 20.0f)); 

    // --- YENİ: CAN YAZISI ---
    sf::Text healthText(font);
    healthText.setCharacterSize(30);            
    healthText.setFillColor(sf::Color::Green);   
    healthText.setPosition(sf::Vector2f(20.0f, 60.0f)); 

    // --- YENİ: GAME OVER YAZISI ---
    sf::Text gameOverText(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nYeniden baslamak icin R'ye bas");
    gameOverText.setPosition(sf::Vector2f(100.0f, 220.0f));

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

    int score = 0;
    int health = 5;       // Başlangıçta 5 can hakkı
    bool isPassed = false; 
    bool isGameOver = false; 

    // Oyun Döngüsü
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // --- GAME OVER KONTROLÜ ---
        if (isGameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                // Oyunu sıfırla
                score = 0;
                health = 5;
                obstacleSpeed = 5.0f;
                obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
                player.resetPosition(sf::Vector2f(200.0f, 100.0f));
                isPassed = false;
                isGameOver = false;
            }
            
            window.clear(sf::Color::Black);
            window.draw(gameOverText);
            window.display();
            continue; 
        }

        player.update();

        // Engel Hareketi
        obstacle.move(sf::Vector2f(-obstacleSpeed, 0.0f));

        // Skor Kontrolü
        if (obstacle.getPosition().x < 200.0f && !isPassed) {
            score++;           
            isPassed = true;   
        }

        if (obstacle.getPosition().x < -40.0f) {
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
            obstacleSpeed += 0.2f; 
            isPassed = false;  
        }

        // Çarpışma Kontrolü (Can Azaltma)
        sf::FloatRect playerBounds = player.getBounds();
        sf::FloatRect obstacleBounds = obstacle.getGlobalBounds();

        if (playerBounds.findIntersection(obstacleBounds).has_value()) {
            health--; // Her çarpmada 1 can düşür
            player.resetPosition(sf::Vector2f(200.0f, 100.0f)); 
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f)); 
            isPassed = false;

            if (health <= 0) {
                isGameOver = true; // Canlar bitince oyunu dondur
            }
        }

        // Yazıları Güncelle
        scoreText.setString("Skor: " + std::to_string(score));
        healthText.setString("Can: " + std::to_string(health)); 

        if (health <= 2) healthText.setFillColor(sf::Color::Red);
        else healthText.setFillColor(sf::Color::Green);

        // Çizim Aşaması
        window.clear(sf::Color::Black);
        
        window.draw(ground);            
        window.draw(obstacle);          
        player.draw(window);            
        window.draw(scoreText); 
        window.draw(healthText); 
        
        window.display();               
    }

    return 0;
}