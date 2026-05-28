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

    // Yerdeki Sabit Kırmızı Engel
    sf::RectangleShape obstacle(sf::Vector2f(40.0f, 40.0f));
    obstacle.setFillColor(sf::Color::Red);
    obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
    float baseObstacleSpeed = 5.0f; 
    float currentObstacleSpeed = baseObstacleSpeed;

    // --- UÇAN DÜŞMAN VE DİNAMİK SINIRLARI ---
    sf::RectangleShape flyingEnemy(sf::Vector2f(40.0f, 30.0f));
    flyingEnemy.setFillColor(sf::Color::Magenta);
    // İlk olarak ekranın sağ dışından (900X) başlasın
    flyingEnemy.setPosition(sf::Vector2f(900.0f, 380.0f)); 
    
    float flyingEnemySpeed = 2.0f; // Kendi içindeki devriye hızı
    float patrolLeft = 800.0f;     // Başlangıç sol sınırı
    float patrolRight = 1000.0f;   // Başlangıç sağ sınırı

    // Altınların Listesi
    std::vector<sf::CircleShape> coins;
    for (int i = 0; i < 4; i++) {
        sf::CircleShape c(15.0f); 
        c.setFillColor(sf::Color::Yellow);
        c.setPosition(sf::Vector2f(950.0f + (i * 250.0f), 515.0f)); 
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
                currentObstacleSpeed = baseObstacleSpeed;
                flyingEnemySpeed = 2.0f;
                patrolLeft = 800.0f;
                patrolRight = 1000.0f;
                obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
                flyingEnemy.setPosition(sf::Vector2f(900.0f, 380.0f));
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

        // Level Sistemi
        if (score >= 30 && score < 60 && level == 1) {
            level = 2;
            currentObstacleSpeed = baseObstacleSpeed + 2.0f;
        } 
        else if (score >= 60 && level == 2) {
            level = 3;
            currentObstacleSpeed = baseObstacleSpeed + 4.0f;
        }

        // Kırmızı Yer Engeli Hareketi
        obstacle.move(sf::Vector2f(-currentObstacleSpeed, 0.0f));
        if (obstacle.getPosition().x < -40.0f) {
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f));
        }

        // --- ⭐ HEM DEVRİYE ATAN HEM DE SOLA AKAN YAPAY ZEKA ⭐ ---
        // 1. Devriye sınırlarını oyun hızıyla sola kaydırıyoruz
        patrolLeft -= currentObstacleSpeed;
        patrolRight -= currentObstacleSpeed;

        // 2. Düşmanı hem sola kaydırıyoruz hem de kendi devriye hızını ekliyoruz
        flyingEnemy.move(sf::Vector2f(-currentObstacleSpeed + flyingEnemySpeed, 0.0f));

        // 3. Kendi sınırlarına çarptı mı kontrolü
        if (flyingEnemy.getPosition().x < patrolLeft) {
            flyingEnemySpeed = std::abs(flyingEnemySpeed); // Sağa dön
        } else if (flyingEnemy.getPosition().x > patrolRight) {
            flyingEnemySpeed = -std::abs(flyingEnemySpeed); // Sola dön
        }

        // 4. Eğer uçan düşman ekrandan tamamen çıkıp sola gittiyse, sağdan yeniden doğsun
        if (flyingEnemy.getPosition().x < -40.0f) {
            flyingEnemy.setPosition(sf::Vector2f(950.0f, 380.0f));
            patrolLeft = 850.0f;
            patrolRight = 1050.0f;
        }

        sf::FloatRect playerBounds = player.getBounds();

        // Yer Engeli Çarpışması
        if (playerBounds.findIntersection(obstacle.getGlobalBounds()).has_value()) {
            health--; 
            player.resetPosition(sf::Vector2f(200.0f, 100.0f)); 
            obstacle.setPosition(sf::Vector2f(850.0f, 510.0f)); 
            if (health <= 0) isGameOver = true; 
        }

        // Uçan Düşman Çarpışması
        if (playerBounds.findIntersection(flyingEnemy.getGlobalBounds()).has_value()) {
            health--; 
            player.resetPosition(sf::Vector2f(200.0f, 100.0f)); 
            // Çarpınca düşmanı da ileri ışınlayalım ki üst üste can gitmesin
            flyingEnemy.setPosition(sf::Vector2f(950.0f, 380.0f));
            patrolLeft = 850.0f;
            patrolRight = 1050.0f;
            if (health <= 0) isGameOver = true; 
        }

        // Altınların Akışı ve Toplanması
        for (size_t i = 0; i < coins.size(); i++) {
            coins[i].move(sf::Vector2f(-currentObstacleSpeed, 0.0f));

            sf::FloatRect coinBounds = coins[i].getGlobalBounds();

            if (playerBounds.findIntersection(coinBounds).has_value()) {
                score += 10; 
                coins.erase(coins.begin() + i); 
                break; 
            }

            if (coinBounds.position.x < -40.0f) {
                coins[i].setPosition(sf::Vector2f(850.0f + (i * 100.0f), 515.0f));
            }
        }

        uiText.setString("Puan: " + std::to_string(score) + "   Can: " + std::to_string(health) + "   Level: " + std::to_string(level));

        window.clear(sf::Color::Black);
        
        window.draw(ground);            
        window.draw(obstacle);          
        window.draw(flyingEnemy); 
        
        for (const auto& c : coins) {
            window.draw(c);
        }

        player.draw(window);            
        window.draw(uiText); 
        
        window.display();               
    }

    return 0;
}