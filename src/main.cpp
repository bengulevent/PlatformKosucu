#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 
#include <vector> 

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Runner");
    window.setFramerateLimit(60); 

    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        font.openFromFile("/System/Library/Fonts/Courier.dfont");
    }

    sf::Text uiText(font);
    uiText.setCharacterSize(26);            
    uiText.setFillColor(sf::Color::White);  
    uiText.setPosition(sf::Vector2f(20.0f, 20.0f)); 

    sf::Text gameOverText(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nYeniden baslamak icin R'ye bas");
    gameOverText.setPosition(sf::Vector2f(100.0f, 220.0f));

    sf::RectangleShape ground(sf::Vector2f(800.0f, 50.0f));
    ground.setFillColor(sf::Color(50, 50, 50));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(200.0f, 100.0f));

    sf::RectangleShape obstacle(sf::Vector2f(40.0f, 40.0f));
    obstacle.setFillColor(sf::Color::Red);
    obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
    float obstacleSpeed = 5.0f;

    // Altınların Listesi
    std::vector<sf::CircleShape> coins;
    for (int i = 0; i < 4; i++) {
        sf::CircleShape c(15.0f); 
        c.setFillColor(sf::Color::Yellow);
        c.setPosition(sf::Vector2f(950.0f + (i * 250.0f), 515.0f)); // Aralarını biraz açtım karışmasınlar
        coins.push_back(c);
    }

    int score = 0;
    int health = 5;       
    int level = 1; 
    bool isGameOver = false; 

    // Oyun Döngüsü
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (isGameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                score = 0;
                health = 5;
                level = 1;
                obstacleSpeed = 5.0f;
                obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
                player.resetPosition(sf::Vector2f(200.0f, 100.0f));
                
                coins.clear();
                for (int i = 0; i < 4; i++) {
                    sf::CircleShape c(15.0f);
                    c.setFillColor(sf::Color::Yellow);
                    c.setPosition(sf::Vector2f(950.0f + (i * 250.0f), 515.0f));
                    coins.push_back(c);
                }
                isGameOver = false;
            }
            
            window.clear(sf::Color::Black);
            window.draw(gameOverText);
            window.display();
            continue; 
        }

        player.update();

        // Kırmızı Engel Hareketi
        obstacle.move(sf::Vector2f(-obstacleSpeed, 0.0f));
        if (obstacle.getPosition().x < -40.0f) {
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
            obstacleSpeed += 0.2f; 
        }

        // --- 🔴 KRAL KANUNU 1: KIRMIZI ENGELE ÇARPINCA CAN DÜŞER ---
        // SFML 3.0 findIntersection mantığıyla tam çarpışmayı canlı okuyoruz
        sf::FloatRect playerBounds = player.getBounds();
        sf::FloatRect obstacleBounds = obstacle.getGlobalBounds();

        if (playerBounds.findIntersection(obstacleBounds).has_value()) {
            health--; // Sadece can düşecek!
            player.resetPosition(sf::Vector2f(200.0f, 100.0f)); 
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f)); 
            
            // Altınları da sıfırlayalım ki çarpma anında iç içe girmesinler
            coins.clear();
            for (int i = 0; i < 4; i++) {
                sf::CircleShape c(15.0f);
                c.setFillColor(sf::Color::Yellow);
                c.setPosition(sf::Vector2f(950.0f + (i * 250.0f), 515.0f));
                coins.push_back(c);
            }

            if (health <= 0) {
                isGameOver = true; 
            }
        }

        // --- 🟡 KRAL KANUNU 2: SARI ALTINA DEĞİNCE PUAN ARTAR ---
        for (size_t i = 0; i < coins.size(); i++) {
            coins[i].move(sf::Vector2f(-obstacleSpeed, 0.0f)); // Altınlar da sola aksın

            sf::FloatRect coinBounds = coins[i].getGlobalBounds();

            // Altın sadece oyuncunun kutusuna değerse toplanır (Engel koordinatından tamamen bağımsız!)
            if (playerBounds.findIntersection(coinBounds).has_value()) {
                score += 10; // Puanı kap
                coins.erase(coins.begin() + i); // Altını yok et
                break; 
            }

            // Altın ekrandan çıkarsa sağdan geri doğsun
            if (coinBounds.position.x < -40.0f) {
                coins[i].setPosition(sf::Vector2f(850.0f + (i * 100.0f), 515.0f));
            }
        }

        uiText.setString("Puan: " + std::to_string(score) + "   Can: " + std::to_string(health) + "   Level: " + std::to_string(level));

        window.clear(sf::Color::Black);
        
        window.draw(ground);            
        window.draw(obstacle);          
        
        for (const auto& c : coins) {
            window.draw(c);
        }

        player.draw(window);            
        window.draw(uiText); 
        
        window.display();               
    }

    return 0;
}