#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 
#include <vector> 
#include <cmath>
#include <iostream>

// Haritadaki düşman nesnelerinin koordinat, hız ve devriye sınırlarını tutan veri yapısı
struct CustomEnemy {
    sf::RectangleShape shape;
    int type; // 0: Sağa-Sola devriye, 1: Aşağı-Yukarı devriye
    float speedX;
    float speedY;
    float startX;
    float startY;
    float patrolRange;
};

// --- BÖLÜM TASARIM MOTORU ---
// Seçilen seviyeye göre statik harita bileşenlerini ve aktörleri yükler
void generateFixedLevel(int level, std::vector<sf::RectangleShape>& platforms, std::vector<CustomEnemy>& enemies, std::vector<sf::CircleShape>& coins, sf::RectangleShape& levelGate) {
    platforms.clear();
    enemies.clear();
    coins.clear();

    if (level == 1) {
        // --- LEVEL 1: TANIŞMA PARKURU ---
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

        // Yatay uçan düşmanlar platform bloklarının 35 piksel yukarısına konumlandırıldı
        CustomEnemy e1; e1.type = 0; e1.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e1.shape.setFillColor(sf::Color::Magenta); e1.startX = 450.0f; e1.startY = 360.0f; 
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 3.0f; e1.speedY = 0.0f; e1.patrolRange = 100.0f;
        enemies.push_back(e1);

        CustomEnemy e2; e2.type = 0; e2.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e2.shape.setFillColor(sf::Color::Magenta); e2.startX = 950.0f; e2.startY = 370.0f; 
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = 3.0f; e2.speedY = 0.0f; e2.patrolRange = 100.0f;
        enemies.push_back(e2);
    } 
    else if (level == 2) {
        // --- LEVEL 2: ASİMETRİK YÜKSEKLİKLER VE DİKEY CANAVARLAR ---
        std::vector<sf::Vector2f> platPositions = {
            {200.0f, 480.0f}, {380.0f, 390.0f}, {550.0f, 320.0f}, 
            {750.0f, 420.0f}, {980.0f, 350.0f}, {1200.0f, 460.0f}, {1400.0f, 380.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(120.0f, 12.0f)); 
            plat.setFillColor(sf::Color(100, 149, 237)); 
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Yatay düşmanlar blokların üst sınırından yukarı taşındı
        CustomEnemy e1; e1.type = 0; e1.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e1.shape.setFillColor(sf::Color::Magenta); e1.startX = 380.0f; e1.startY = 330.0f; 
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 3.5f; e1.speedY = 0.0f; e1.patrolRange = 70.0f;
        enemies.push_back(e1);

        CustomEnemy e2; e2.type = 0; e2.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e2.shape.setFillColor(sf::Color::Magenta); e2.startX = 1200.0f; e2.startY = 400.0f; 
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = 3.5f; e2.speedY = 0.0f; e2.patrolRange = 70.0f;
        enemies.push_back(e2);

        // Dikey düzlemde devriye gezen sarı renkli düşmanlar
        CustomEnemy eV1; eV1.type = 1; eV1.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        eV1.shape.setFillColor(sf::Color::Yellow); eV1.startX = 650.0f; eV1.startY = 250.0f;
        eV1.shape.setPosition(sf::Vector2f(eV1.startX, eV1.startY)); eV1.speedX = 0.0f; eV1.speedY = 4.0f; eV1.patrolRange = 120.0f;
        enemies.push_back(eV1);
    } 
    else if (level == 3) {
        // --- LEVEL 3: HARDCORE TAPINAK PARKURU ---
        std::vector<sf::Vector2f> platPositions = {
            {150.0f, 450.0f}, {320.0f, 360.0f}, {500.0f, 420.0f}, 
            {700.0f, 330.0f}, {900.0f, 450.0f}, {1100.0f, 360.0f}, {1300.0f, 410.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(100.0f, 15.0f)); 
            plat.setFillColor(sf::Color(178, 34, 34)); 
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Yatay düşmanlar için dikey boşluklar eklendi
        CustomEnemy e1; e1.type = 0; e1.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e1.shape.setFillColor(sf::Color::Magenta); e1.startX = 320.0f; e1.startY = 300.0f; 
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 4.2f; e1.speedY = 0.0f; e1.patrolRange = 60.0f;
        enemies.push_back(e1);

        CustomEnemy e2; e2.type = 0; e2.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e2.shape.setFillColor(sf::Color::Magenta); e2.startX = 1100.0f; e2.startY = 300.0f; 
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = 4.2f; e2.speedY = 0.0f; e2.patrolRange = 60.0f;
        enemies.push_back(e2);

        // Dikey düzlemde devriye gezen asabi düşmanlar
        CustomEnemy eV1; eV1.type = 1; eV1.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        eV1.shape.setFillColor(sf::Color::Yellow); eV1.startX = 250.0f; eV1.startY = 200.0f;
        eV1.shape.setPosition(sf::Vector2f(eV1.startX, eV1.startY)); eV1.speedX = 0.0f; eV1.speedY = 5.0f; eV1.patrolRange = 150.0f;
        enemies.push_back(eV1);

        CustomEnemy eV2; eV2.type = 1; eV2.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        eV2.shape.setFillColor(sf::Color::Yellow); eV2.startX = 820.0f; eV2.startY = 250.0f;
        eV2.shape.setPosition(sf::Vector2f(eV2.startX, eV2.startY)); eV2.speedX = 0.0f; eV2.speedY = 5.0f; eV2.patrolRange = 130.0f;
        enemies.push_back(eV2);
    }

    // Altınların konumlandırma döngüsü
    for (const auto& plat : platforms) {
        sf::CircleShape c(10.0f); 
        c.setFillColor(sf::Color::Yellow);
        c.setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f) - 10.0f, plat.getPosition().y - 32.0f));
        coins.push_back(c);
    }

    levelGate.setPosition(sf::Vector2f(platforms.back().getPosition().x + 220.0f, 400.0f));
}

int main() {
    // Mac Retina ekranlardaki piksellenme bulanıklığını önleyen gelişmiş pencere yapılandırması
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0; // Grafik kartı yumuşatma filtresi kapatıldı

    sf::RenderWindow window(desktopMode, "Action Platformer", sf::State::Fullscreen, settings);
    window.setFramerateLimit(60); 

    // Oyun içi mantıksal çözünürlüğün 800x600 olarak sabitlenmesi
    sf::View camera(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));

    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        font.openFromFile("/System/Library/Fonts/Courier.dfont");
    }

    sf::Text uiText(font);
    uiText.setCharacterSize(26);            
    uiText.setFillColor(sf::Color::White);  

    sf::Text centerText(font);
    centerText.setCharacterSize(42);
    centerText.setFillColor(sf::Color::Red);

    sf::RectangleShape ground(sf::Vector2f(5000.0f, 50.0f));
    ground.setFillColor(sf::Color(50, 50, 50));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    sf::RectangleShape levelGate(sf::Vector2f(20.0f, 150.0f));
    levelGate.setFillColor(sf::Color::Green);

    // --- GENİŞLETİLMİŞ VE KESİNTİSİZ ARKA PLAN SİSTEMİ ---
    sf::Texture bgTexture;
    bool hasBg = bgTexture.loadFromFile("assets/sunset_cloud.png"); 
    
    if (!hasBg) {
        // Eğer resim yüklenemezse siyah ekranda kalma, terminale hatayı bas
        std::cout << "\n========================================================" << std::endl;
        std::cout << "HATA: assets/sunset_cloud.png yuklenemedi!" << std::endl;
        std::cout << "Lutfen assets klasorunun icinde 'sunset_cloud.png' oldugundan emin ol!" << std::endl;
        std::cout << "========================================================\n" << std::endl;
        return -1; 
    }

    // Keskin piksel filtresi aktif hale getirildi
    bgTexture.setSmooth(false); 

    sf::Sprite bgSprite1(bgTexture);
    sf::Sprite bgSprite2(bgTexture);
    
    float bgWidth = bgTexture.getSize().x * (600.0f / bgTexture.getSize().y);
    float bgScaleY = 600.0f / bgTexture.getSize().y;
    bgSprite1.setScale(sf::Vector2f(bgScaleY, bgScaleY)); 
    bgSprite2.setScale(sf::Vector2f(bgScaleY, bgScaleY)); 

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

    // --- ANA OYUN DÖNGÜSÜ ---
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x - 5.0f, player.getBounds().position.y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x + 5.0f, player.getBounds().position.y));
        }

        bool jumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space); 

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
            sf::CircleShape newBullet(5.0f);
            newBullet.setFillColor(sf::Color::Red);
            newBullet.setPosition(sf::Vector2f(player.getBounds().position.x + player.getBounds().size.x, player.getBounds().position.y + 20.0f));
            bullets.push_back(newBullet);
            shootClock.restart();
        }

        player.update();

        if (platformJumpVelocity < 0.0f) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x, player.getBounds().position.y + platformJumpVelocity));
            platformJumpVelocity += 0.6f; 
        }

        float playerX = player.getBounds().position.x;
        if (playerX > 400.0f) {
            camera.setCenter(sf::Vector2f(playerX, 300.0f));
        } else {
            camera.setCenter(sf::Vector2f(400.0f, 300.0f));
        }
        window.setView(camera); 

        if (bgWidth > 0.0f) {
            float bgX = camera.getCenter().x * 0.2f;
            float offset = std::fmod(bgX, bgWidth);
            bgSprite1.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset, 0.0f));
            bgSprite2.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset + bgWidth, 0.0f));
        }

        sf::FloatRect playerBounds = player.getBounds();

        for (auto& enemy : enemies) {
            if (enemy.type == 0) {
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

        // --- LEVEL GEÇİŞ MANTIĞI ---
        showWarning = false; 
        if (playerBounds.findIntersection(levelGate.getGlobalBounds()).has_value()) {
            if (score >= 60) { 
                if (currentLevel < 3) {
                    currentLevel++; 
                    score = 0; // Seviye başında net sıfırlama işlemi
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

        // Platform çarpışma mekaniği
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

        // Mermi çarpışma döngüsü
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

        // --- DÜŞMAN HASAR KONTROLÜ VE DOSYA YENİDEN YÜKLEME ---
        for (const auto& enemy : enemies) {
            if (playerBounds.findIntersection(enemy.shape.getGlobalBounds()).has_value()) {
                health--;
                score = 0; 
                // Hasar durumunda seviyedeki tüm altınlar ve düşmanlar sıfırdan haritaya yüklenir
                generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate);
                player.resetPosition(sf::Vector2f(100.0f, 400.0f)); 
                bullets.clear();
                if (health <= 0) isGameOver = true;
                break;
            }
        }

        // Altın toplama döngüsü
        for (size_t i = 0; i < coins.size(); ) {
            if (playerBounds.findIntersection(coins[i].getGlobalBounds()).has_value()) {
                coins.erase(coins.begin() + i); 
                score += 10; 
            } else {
                i++;
            }
        }

        std::string uiStr = "Puan: " + std::to_string(score) + "   Can: " + std::to_string(health) + "   Bolum: " + std::to_string(currentLevel);
        if (showWarning) uiStr += "  [ KAPI ICIN 60 PUAN GEREKLI! ]";
        uiText.setString(uiStr);
        uiText.setPosition(sf::Vector2f(camera.getCenter().x - 380.0f, 20.0f));

        window.clear(sf::Color::Black);
        
        if (bgWidth > 0.0f) {
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
        
        window.display();               
    }

    return 0;
}