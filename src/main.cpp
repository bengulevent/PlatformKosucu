#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 
#include <vector> 
#include <cmath>
#include <iostream>

// Geliştirilmiş Düşman Yapısı (Uçan Kalp Canavarları Dahil)
struct CustomEnemy {
    sf::RectangleShape shape;
    int type; // 0: Yatay Devriye, 1: Dikey Uçan Canavar, 2: Kalp Fobisi (Hızlı Uçan Canavar)
    float speedX;
    float speedY;
    float startX;
    float startY;
    float patrolRange;
};

// --- BÖLÜM TASARIM MOTORU ---
void generateFixedLevel(int level, std::vector<sf::RectangleShape>& platforms, std::vector<CustomEnemy>& enemies, std::vector<sf::CircleShape>& coins, sf::RectangleShape& levelGate) {
    platforms.clear();
    enemies.clear();
    coins.clear();

    if (level == 1) {
        // --- LEVEL 1 ---
        std::vector<sf::Vector2f> platPositions = {
            {250.0f, 450.0f}, {450.0f, 420.0f}, {700.0f, 460.0f}, 
            {950.0f, 430.0f}, {1200.0f, 400.0f}, {1450.0f, 450.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(140.0f, 10.0f)); 
            plat.setFillColor(sf::Color(120, 120, 120)); 
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Standart Yatay Canavar
        CustomEnemy e1; e1.type = 0; e1.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e1.shape.setFillColor(sf::Color::Magenta); e1.startX = 450.0f; e1.startY = 395.0f; 
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 3.0f; e1.speedY = 0.0f; e1.patrolRange = 100.0f;
        enemies.push_back(e1);

        // Kalp Fobisi Uçan Canavar
        CustomEnemy e_heart; e_heart.type = 2; e_heart.shape.setSize(sf::Vector2f(25.0f, 25.0f));
        e_heart.shape.setFillColor(sf::Color(139, 0, 139)); 
        e_heart.startX = 850.0f; e_heart.startY = 280.0f;
        e_heart.shape.setPosition(sf::Vector2f(e_heart.startX, e_heart.startY)); e_heart.speedX = 0.0f; e_heart.speedY = 2.5f; e_heart.patrolRange = 80.0f;
        enemies.push_back(e_heart);
    } 
    else if (level == 2) {
        // --- LEVEL 2 ---
        std::vector<sf::Vector2f> platPositions = {
            {200.0f, 480.0f}, {380.0f, 390.0f}, {550.0f, 320.0f}, 
            {750.0f, 420.0f}, {980.0f, 350.0f}, {1200.0f, 460.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(120.0f, 12.0f)); 
            plat.setFillColor(sf::Color(100, 149, 237)); 
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Uçan Canavar (Dikey)
        CustomEnemy e1; e1.type = 1; e1.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        e1.shape.setFillColor(sf::Color::Cyan); e1.startX = 480.0f; e1.startY = 250.0f;
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 0.0f; e1.speedY = 2.5f; e1.patrolRange = 80.0f;
        enemies.push_back(e1);

        // Kalp Fobisi Canavarı (Yatay ve Hızlı)
        CustomEnemy e2; e2.type = 2; e2.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        e2.shape.setFillColor(sf::Color(199, 21, 133)); e2.startX = 800.0f; e2.startY = 350.0f;
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = 5.0f; e2.speedY = 0.0f; e2.patrolRange = 130.0f;
        enemies.push_back(e2);

        // Ekstra Uçan Canavar
        CustomEnemy e3; e3.type = 1; e3.shape.setSize(sf::Vector2f(25.0f, 25.0f));
        e3.shape.setFillColor(sf::Color::Red); e3.startX = 1100.0f; e3.startY = 280.0f;
        e3.shape.setPosition(sf::Vector2f(e3.startX, e3.startY)); e3.speedX = 0.0f; e3.speedY = 3.0f; e3.patrolRange = 90.0f;
        enemies.push_back(e3);
    } 
    else if (level == 3) {
        // --- LEVEL 3 ---
        std::vector<sf::Vector2f> platPositions = {
            {150.0f, 450.0f}, {350.0f, 360.0f}, {550.0f, 280.0f},
            {750.0f, 360.0f}, {950.0f, 440.0f}, {1150.0f, 350.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(100.0f, 15.0f)); 
            plat.setFillColor(sf::Color(178, 34, 34)); 
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Uçan Canavar 1 (Dikey)
        CustomEnemy e1; e1.type = 1; e1.shape.setSize(sf::Vector2f(25.0f, 25.0f));
        e1.shape.setFillColor(sf::Color::Red); e1.startX = 280.0f; e1.startY = 300.0f;
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 0.0f; e1.speedY = 3.5f; e1.patrolRange = 100.0f;
        enemies.push_back(e1);

        // Kalp Fobisi
        CustomEnemy e2; e2.type = 2; e2.shape.setSize(sf::Vector2f(35.0f, 35.0f));
        e2.shape.setFillColor(sf::Color(148, 0, 211)); e2.startX = 550.0f; e2.startY = 220.0f;
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = 6.0f; e2.speedY = 0.0f; e2.patrolRange = 100.0f;
        enemies.push_back(e2);

        // Uçan Canavar 3
        CustomEnemy e3; e3.type = 1; e3.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        e3.shape.setFillColor(sf::Color::Red); e3.startX = 1050.0f; e3.startY = 320.0f;
        e3.shape.setPosition(sf::Vector2f(e3.startX, e3.startY)); e3.speedX = 0.0f; e3.speedY = 4.5f; e3.patrolRange = 110.0f;
        enemies.push_back(e3);

        // Havada Ekstra Kalp Kabusu Canavarı
        CustomEnemy e4; e4.type = 2; e4.shape.setSize(sf::Vector2f(30.0f, 20.0f));
        e4.shape.setFillColor(sf::Color::Yellow); e4.startX = 750.0f; e4.startY = 180.0f;
        e4.shape.setPosition(sf::Vector2f(e4.startX, e4.startY)); e4.speedX = 5.5f; e4.speedY = 0.0f; e4.patrolRange = 150.0f;
        enemies.push_back(e4);
    }

    for (const auto& plat : platforms) {
        sf::CircleShape c(10.0f); 
        c.setFillColor(sf::Color::Yellow);
        c.setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f) - 10.0f, plat.getPosition().y - 32.0f));
        coins.push_back(c);
    }

    levelGate.setPosition(sf::Vector2f(platforms.back().getPosition().x + 220.0f, 400.0f));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Kosucu", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60); 

    sf::View camera(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));

    // Font Yükleme Mekanizması
    sf::Font font;
    if (!font.openFromFile("assets/pixel.ttf")) {
        if (!font.openFromFile("../assets/pixel.ttf")) {
            if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                font.openFromFile("/System/Library/Fonts/Courier.dfont");
            }
        }
    }

    // --- KALP GÖRSELİ İÇİN DOKU YÜKLEMESİ ---
    sf::Texture heartTexture;
    bool hasHeartImg = false;
    if (heartTexture.loadFromFile("assets/pixel_heart.png")) {
        hasHeartImg = true;
    } else if (heartTexture.loadFromFile("../assets/pixel_heart.png")) {
        hasHeartImg = true;
    }

    sf::Sprite heartSprite(heartTexture);
    if (hasHeartImg) {
        heartSprite.setScale(sf::Vector2f(0.06f, 0.06f)); 
    }

    // --- BORDOMSU ÇOK KOYU PEMBE VE KALIN YAZI SEÇİMİ ---
    sf::Color deepBordoPink(139, 0, 70); 

    sf::Text uiText(font);
    uiText.setCharacterSize(28); 
    uiText.setFillColor(deepBordoPink);  
    uiText.setStyle(sf::Text::Bold); 

    sf::Text centerText(font);
    centerText.setCharacterSize(42);
    centerText.setFillColor(sf::Color::Red);
    centerText.setStyle(sf::Text::Bold);

    sf::RectangleShape ground(sf::Vector2f(5000.0f, 50.0f));
    ground.setFillColor(sf::Color(100, 100, 100));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    sf::RectangleShape levelGate(sf::Vector2f(20.0f, 150.0f));
    levelGate.setFillColor(sf::Color::Green);

    sf::Texture bgTexture;
    bool hasBg = bgTexture.loadFromFile("assets/sunset_cloud.png");

    sf::Sprite bgSprite1(bgTexture);
    sf::Sprite bgSprite2(bgTexture);
    
    float bgWidth = 0.0f;
    if (hasBg) {
        float bgScaleY = 600.0f / bgTexture.getSize().y;
        bgSprite1.setScale(sf::Vector2f(bgScaleY, bgScaleY)); 
        bgSprite2.setScale(sf::Vector2f(bgScaleY, bgScaleY)); 
        bgWidth = bgTexture.getSize().x * bgScaleY;
    }

    std::vector<sf::RectangleShape> platforms;
    std::vector<CustomEnemy> enemies;
    std::vector<sf::CircleShape> coins;

    int currentLevel = 1;
    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate);

    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(100.0f, 400.0f));

    std::vector<sf::CircleShape> bullets; 
    sf::Clock shootClock; 
    float shootCooldown = 0.25f; 

    int score = 0;
    int health = 5; 
    bool isGameOver = false; 
    bool isGameWon = false;
    bool showWarning = false;
    float platformJumpVelocity = 0.0f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }

        if (isGameOver) {
            centerText.setString("GAME OVER! Yandiniz.");
            centerText.setPosition(sf::Vector2f(camera.getCenter().x - 180.0f, 250.0f));
            window.clear(sf::Color::Black);
            window.draw(centerText);
            window.display();
            continue; 
        }

        if (isGameWon) {
            centerText.setString("TEBRIKLER! OYUNU KAZANDINIZ.");
            centerText.setPosition(sf::Vector2f(camera.getCenter().x - 250.0f, 250.0f));
            window.clear(sf::Color::Black);
            window.draw(centerText);
            window.display();
            continue;
        }

        sf::FloatRect playerBounds = player.getBounds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            player.resetPosition(sf::Vector2f(playerBounds.position.x - 5.0f, playerBounds.position.y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            player.resetPosition(sf::Vector2f(playerBounds.position.x + 5.0f, playerBounds.position.y));
        }

        bool jumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space); 

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
            sf::CircleShape newBullet(5.0f);
            newBullet.setFillColor(sf::Color::Red);
            newBullet.setPosition(sf::Vector2f(playerBounds.position.x + playerBounds.size.x, playerBounds.position.y + 20.0f));
            bullets.push_back(newBullet);
            shootClock.restart();
        }

        player.update();

        if (platformJumpVelocity < 0.0f) {
            player.resetPosition(sf::Vector2f(playerBounds.position.x, playerBounds.position.y + platformJumpVelocity));
            platformJumpVelocity += 0.6f; 
        }

        playerBounds = player.getBounds(); 
        float playerX = playerBounds.position.x;
        if (playerX > 400.0f) {
            camera.setCenter(sf::Vector2f(playerX, 300.0f));
        } else {
            camera.setCenter(sf::Vector2f(400.0f, 300.0f));
        }
        window.setView(camera); 

        if (hasBg && bgWidth > 0.0f) {
            float bgX = camera.getCenter().x * 0.2f;
            float offset = std::fmod(bgX, bgWidth);
            bgSprite1.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset, 0.0f));
            bgSprite2.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset + bgWidth, 0.0f));
        }

        for (auto& enemy : enemies) {
            if (enemy.type == 0 || enemy.type == 2) {
                enemy.shape.move(sf::Vector2f(enemy.speedX, 0.0f));
                if (std::abs(enemy.shape.getPosition().x - enemy.startX) > enemy.patrolRange) {
                    enemy.speedX *= -1.0f;
                }
            } else if (enemy.type == 1) {
                enemy.shape.move(sf::Vector2f(0.0f, enemy.speedY));
                if (std::abs(enemy.shape.getPosition().y - enemy.startY) > enemy.patrolRange) {
                    enemy.speedY *= -1.0f;
                }
            }
        }

        showWarning = false; 
        if (playerBounds.findIntersection(levelGate.getGlobalBounds()).has_value()) {
            if (score >= 60) { 
                if (currentLevel < 3) {
                    currentLevel++; 
                    score = 0; 
                    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate);
                    player.resetPosition(sf::Vector2f(100.0f, 400.0f)); 
                    platformJumpVelocity = 0.0f;
                    bullets.clear();
                } else {
                    isGameWon = true; 
                }
            } else {
                showWarning = true; 
            }
        }

        // Zemin çarpışma kontrolü
        sf::FloatRect groundBounds = ground.getGlobalBounds();
        if (playerBounds.findIntersection(groundBounds).has_value()) {
            if (playerBounds.position.y + playerBounds.size.y <= groundBounds.position.y + 14.0f) {
                player.resetPosition(sf::Vector2f(playerBounds.position.x, groundBounds.position.y - playerBounds.size.y));
                playerBounds = player.getBounds(); 
                if (jumpKeyPressed) {
                    platformJumpVelocity = -14.5f; 
                } else if (platformJumpVelocity > 0.0f) {
                    platformJumpVelocity = 0.0f; 
                }
            }
        }

        // Platform çarpışma kontrolü
        for (const auto& platform : platforms) {
            sf::FloatRect platBounds = platform.getGlobalBounds();
            if (playerBounds.findIntersection(platBounds).has_value()) {
                if (playerBounds.position.y + playerBounds.size.y <= platBounds.position.y + 14.0f) {
                    player.resetPosition(sf::Vector2f(playerBounds.position.x, platBounds.position.y - playerBounds.size.y));
                    playerBounds = player.getBounds(); 
                    if (jumpKeyPressed) {
                        platformJumpVelocity = -14.5f; 
                    } else if (platformJumpVelocity > 0.0f) {
                        platformJumpVelocity = 0.0f; 
                    }
                }
            }
        }

        // Mermi döngüsü
        for (size_t i = 0; i < bullets.size(); ) {
            bullets[i].move(sf::Vector2f(8.0f, 0.0f));
            bool bulletRemoved = false;

            for (size_t j = 0; j < enemies.size(); j++) {
                if (bullets[i].getGlobalBounds().findIntersection(enemies[j].shape.getGlobalBounds()).has_value()) {
                    enemies.erase(enemies.begin() + j); 
                    bullets.erase(bullets.begin() + i); 
                    score += 20; 
                    bulletRemoved = true;
                    break;
                }
            }

            if (!bulletRemoved) {
                if (bullets[i].getPosition().x > camera.getCenter().x + 400.0f) {
                    bullets.erase(bullets.begin() + i);
                } else {
                    i++;
                }
            }
        }

        // Hasar alma döngüsü
        for (const auto& enemy : enemies) {
            if (playerBounds.findIntersection(enemy.shape.getGlobalBounds()).has_value()) {
                health--;
                score = 0; 
                generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate);
                player.resetPosition(sf::Vector2f(100.0f, 400.0f)); 
                bullets.clear();
                if (health <= 0) isGameOver = true;
                break;
            }
        }

        // Altın döngüsü
        for (size_t i = 0; i < coins.size(); ) {
            if (playerBounds.findIntersection(coins[i].getGlobalBounds()).has_value()) {
                coins.erase(coins.begin() + i); 
                score += 10; 
            } else {
                i++;
            }
        }

        // --- RETRO ARAYÜZ METİN YAPISI (CAN SİLİNDİ) ---
        std::string uiStr = "LEVEL: " + std::to_string(currentLevel) + "\n" +
                            "SCORE: " + std::to_string(score);
        
        if (showWarning) uiStr += "\n[KAPI ICIN 60 PUAN GEREKLI!]";
        
        uiText.setString(uiStr);
        float uiBaseX = camera.getCenter().x - 380.0f;
        float uiBaseY = 20.0f;
        uiText.setPosition(sf::Vector2f(uiBaseX, uiBaseY));

        window.clear(sf::Color::Black);
        
        if (hasBg && bgWidth > 0.0f) {
            window.draw(bgSprite1);   
            window.draw(bgSprite2);
        }   
        window.draw(ground);            
        window.draw(levelGate); 
        
        for (const auto& enemy : enemies) window.draw(enemy.shape);
        for (const auto& b : bullets) window.draw(b);
        for (const auto& platform : platforms) window.draw(platform);
        for (const auto& c : coins) window.draw(c);

        player.draw(window);            
        window.draw(uiText); 

        // --- SCORE YAZISININ TAM ALTINA HİZALANMIŞ KALPLER ---
        if (hasHeartImg) {
            float startHeartX = uiBaseX;    // LEVEL ve SCORE ile tam sol dikey hizalama
            float heartY = uiBaseY + 74.0f; // SCORE satırının tam altına gelecek boşluk (3. satır hizası)
            
            for (int i = 0; i < health; i++) {
                heartSprite.setPosition(sf::Vector2f(startHeartX + (i * 38.0f), heartY)); 
                window.draw(heartSprite);
            }
        }
        
        window.display();           
    }

    return 0;
}