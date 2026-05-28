#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 
#include <vector> 
#include <random> 

// --- YENİ: MERMİ YAPISI (BULLET STRUCT) ---
struct Bullet {
    sf::CircleShape shape;
    float speed;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platformer Shooter Adventure");
    window.setFramerateLimit(60); 

    sf::View camera(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));

    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<float> heightDist(300.0f, 480.0f);   
    std::uniform_real_distribution<float> widthDist(60.0f, 110.0f);     
    std::uniform_real_distribution<float> spacingDist(140.0f, 220.0f);  
    std::uniform_real_distribution<float> enemyYDist(200.0f, 320.0f);   

    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        font.openFromFile("/System/Library/Fonts/Courier.dfont");
    }

    sf::Text uiText(font);
    uiText.setCharacterSize(26);            
    uiText.setFillColor(sf::Color::White);  

    sf::Text gameOverText(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nYeniden baslamak icin R'ye bas");

    sf::RectangleShape ground(sf::Vector2f(4000.0f, 50.0f));
    ground.setFillColor(sf::Color(50, 50, 50));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    std::vector<sf::RectangleShape> platforms;
    float platX = 220.0f; 
    for (int i = 0; i < 14; i++) { 
        sf::RectangleShape plat(sf::Vector2f(widthDist(gen), 10.0f)); 
        plat.setFillColor(sf::Color(120, 120, 120));
        plat.setPosition(sf::Vector2f(platX, heightDist(gen))); 
        platforms.push_back(plat);
        platX += plat.getSize().x + spacingDist(gen); 
    }

    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(100.0f, 400.0f));

    // Uçan Düşmanlar
    std::vector<sf::RectangleShape> enemies;
    std::vector<float> enemySpeeds = {2.2f, -2.8f, 3.2f, -2.0f, 2.5f};
    std::vector<std::pair<float, float>> patrolBounds = {
        {350.0f, 650.0f}, 
        {800.0f, 1150.0f}, 
        {1400.0f, 1750.0f},
        {2000.0f, 2400.0f},
        {2700.0f, 3100.0f}
    };

    for (int i = 0; i < 5; i++) {
        sf::RectangleShape e(sf::Vector2f(35.0f, 25.0f));
        e.setFillColor(sf::Color::Magenta);
        e.setPosition(sf::Vector2f(patrolBounds[i].first + 40.0f, enemyYDist(gen)));
        enemies.push_back(e);
    }

    // Altınlar
    std::vector<sf::CircleShape> coins;
    for (int i = 0; i < 8; i++) {
        sf::CircleShape c(12.0f);
        c.setFillColor(sf::Color::Yellow);
        c.setPosition(sf::Vector2f(200.0f + (i * 450.0f), 515.0f));
        coins.push_back(c);
    }
    for (const auto& plat : platforms) {
        sf::CircleShape c(10.0f); 
        c.setFillColor(sf::Color::Yellow);
        c.setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f) - 10.0f, plat.getPosition().y - 32.0f));
        coins.push_back(c);
    }

    // --- YENİ: MERMİ LİSTESİ VE SİLAH COOLDOWN DEĞİŞKENLERİ ---
    std::vector<Bullet> bullets;
    sf::Clock shootClock; // Üst üste saniyede milyon tane mermi atmayı engellemek için süreölçer
    float shootCooldown = 0.3f; // İki mermi arası 0.3 saniye beklenecek (Ateş hızı)

    int score = 0;
    int health = 5;       
    bool isGameOver = false; 
    float platformJumpVelocity = 0.0f;

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
                player.resetPosition(sf::Vector2f(100.0f, 400.0f));
                platformJumpVelocity = 0.0f;
                camera.setCenter(sf::Vector2f(400.0f, 300.0f)); 
                bullets.clear(); // Mermileri temizle
                
                enemies.clear(); // Düşmanları yeniden canlandır
                for (int i = 0; i < 5; i++) {
                    sf::RectangleShape e(sf::Vector2f(35.0f, 25.0f));
                    e.setFillColor(sf::Color::Magenta);
                    e.setPosition(sf::Vector2f(patrolBounds[i].first + 40.0f, enemyYDist(gen)));
                    enemies.push_back(e);
                }

                coins.clear();
                for (int i = 0; i < 8; i++) {
                    sf::CircleShape c(12.0f);
                    c.setFillColor(sf::Color::Yellow);
                    c.setPosition(sf::Vector2f(200.0f + (i * 450.0f), 515.0f));
                    coins.push_back(c);
                }
                for (const auto& plat : platforms) {
                    sf::CircleShape c(10.0f);
                    c.setFillColor(sf::Color::Yellow);
                    c.setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f) - 10.0f, plat.getPosition().y - 32.0f));
                    coins.push_back(c);
                }
                isGameOver = false;
            }
            
            window.setView(window.getDefaultView());
            window.clear(sf::Color::Black);
            gameOverText.setPosition(sf::Vector2f(100.0f, 220.0f));
            window.draw(gameOverText);
            window.display();
            continue; 
        }

        bool jumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x - 5.0f, player.getBounds().position.y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x + 5.0f, player.getBounds().position.y));
        }

        // --- YENİ: ATEŞ ETME KONTROLÜ (F TUŞU) ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
            Bullet newBullet;
            newBullet.shape.setRadius(5.0f); // Küçük mermi yuvarlağı
            newBullet.shape.setFillColor(sf::Color::Cyan); // Parlak açık mavi/turkuaz mermi rengi
            
            // Merminin çıkış noktasını karakterin tam ortası yapıyoruz
            sf::FloatRect pB = player.getBounds();
            newBullet.shape.setPosition(sf::Vector2f(pB.position.x + pB.size.x, pB.position.y + (pB.size.y / 2.0f) - 5.0f));
            newBullet.speed = 10.0f; // Merminin uçuş hızı
            
            bullets.push_back(newBullet);
            shootClock.restart(); // Cooldown'ı sıfırla
        }

        player.update();

        if (platformJumpVelocity < 0.0f) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x, player.getBounds().position.y + platformJumpVelocity));
            platformJumpVelocity += 0.6f; 
        }

        // CAMERA SCROLLING
        float playerX = player.getBounds().position.x;
        if (playerX > 400.0f) {
            camera.setCenter(sf::Vector2f(playerX, 300.0f));
        } else {
            camera.setCenter(sf::Vector2f(400.0f, 300.0f));
        }
        window.setView(camera); 

        // Düşman Hareketleri
        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].move(sf::Vector2f(enemySpeeds[i], 0.0f));
            if (enemies[i].getPosition().x < patrolBounds[i].first) enemySpeeds[i] = std::abs(enemySpeeds[i]);
            else if (enemies[i].getPosition().x > patrolBounds[i].second) enemySpeeds[i] = -std::abs(enemySpeeds[i]);
        }

        sf::FloatRect playerBounds = player.getBounds();

        // Platform Çarpışması
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

        // --- YENİ: MERMİLERİN HAREKETİ VE DÜŞMANI YOK ETME MANTIĞI ---
        for (size_t bIdx = 0; bIdx < bullets.size(); bIdx++) {
            bullets[bIdx].shape.move(sf::Vector2f(bullets[bIdx].speed, 0.0f)); // Mermiyi sağa yürüt
            sf::FloatRect bulletBounds = bullets[bIdx].shape.getGlobalBounds();

            bool bulletDestroyed = false;

            // Mermi düşmanlardan birine değdi mi?
            for (size_t eIdx = 0; eIdx < enemies.size(); eIdx++) {
                if (bulletBounds.findIntersection(enemies[eIdx].getGlobalBounds()).has_value()) {
                    score += 20; // Canavarı vurunca ekstra +20 puan kap!
                    enemies.erase(enemies.begin() + eIdx); // Canavarı haritadan sil! 🔥
                    bullets.erase(bullets.begin() + bIdx); // Mermiyi de yok et
                    bulletDestroyed = true;
                    break;
                }
            }

            if (bulletDestroyed) {
                break; 
            }

            // Mermi kameranın çok uzağına (ekran dışına) gittiyse hafıza birikmesin diye sil
            if (bullets[bIdx].shape.getPosition().x > camera.getCenter().x + 500.0f) {
                bullets.erase(bullets.begin() + bIdx);
                break;
            }
        }

        // Düşman Hasar Kontrolü (Hala hayatta olan düşmanlar vurursa)
        for (auto& enemy : enemies) {
            if (playerBounds.findIntersection(enemy.getGlobalBounds()).has_value()) {
                health--; 
                player.resetPosition(sf::Vector2f(camera.getCenter().x - 300.0f, 400.0f)); 
                platformJumpVelocity = 0.0f;
                if (health <= 0) isGameOver = true; 
            }
        }

        // Altın Toplama
        for (size_t i = 0; i < coins.size(); i++) {
            if (playerBounds.findIntersection(coins[i].getGlobalBounds()).has_value()) {
                score += 10; 
                coins.erase(coins.begin() + i); 
                break; 
            }
        }

        uiText.setString("Puan: " + std::to_string(score) + "   Can: " + std::to_string(health));
        uiText.setPosition(sf::Vector2f(camera.getCenter().x - 380.0f, 20.0f));

        // --- ÇİZİM ---
        window.clear(sf::Color::Black);
        
        window.draw(ground);            
        
        // Düşmanları Çiz
        for (const auto& enemy : enemies) {
            window.draw(enemy);
        }
        
        // --- YENİ: MERMİLERİ ÇIZ ---
        for (const auto& b : bullets) {
            window.draw(b.shape);
        }
        
        for (const auto& platform : platforms) {
            window.draw(platform);
        }
        
        for (const auto& c : coins) {
            window.draw(c);
        }

        player.draw(window);            
        window.draw(uiText); 
        
        window.display();               
    }

    return 0;
}