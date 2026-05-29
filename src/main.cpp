#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 
#include <vector> 
#include <cmath>
#include <iostream>

// --- OYUN NESNELERİNİN YAPILARI (STRUCTS) ---

// Haritadaki renkli canavarların koordinat, hız ve devriye sınırlarını tutan yapı
struct CustomEnemy {
    sf::RectangleShape shape;
    int type; // 0: Yatay devriye gezen, 1: Dikey (aşağı-yukarı) uçan canavar
    float speedX;
    float speedY;
    float startX;
    float startY;
    float patrolRange;
};

// SFML 3.0 sürümündeki 'default constructor' hatalarını engellemek için pointer ile tasarladığımız altın yapısı
struct GoldCoin {
    sf::Sprite* sprite; // Bellek hatası vermemesi için sprite nesnesini pointer (işaretçi) olarak tutuyoruz
    sf::FloatRect bounds; // Altının ekrandaki çarpışma kutusu koordinatları
    bool isCollected;   // Oyuncu altını aldı mı almadı mı kontrolü
};

// --- KADEMELİ OLARAK ZORLAŞTIRILAN VE 3. LEVELİ UZATILAN BÖLÜM MOTORU ---
void generateFixedLevel(int level, std::vector<sf::RectangleShape>& platforms, std::vector<CustomEnemy>& enemies, std::vector<GoldCoin>& coins, sf::RectangleShape& levelGate, const sf::Texture& goldTexture) {
    
    // Hafıza sızıntısı (memory leak) olmasın diye bir önceki bölümden kalan altın pointer'larını bellekten temizliyoruz
    for (auto& c : coins) {
        delete c.sprite;
    }
    
    // Tüm listeleri yeni bölüme hazırlamak için sıfırlıyoruz
    platforms.clear();
    enemies.clear();
    coins.clear();

    // --- LEVEL 1: Giriş Seviyesi (2 Canavar) ---
    if (level == 1) {
        std::vector<sf::Vector2f> platPositions = {
            {250.0f, 450.0f}, {450.0f, 420.0f}, {700.0f, 460.0f}, 
            {950.0f, 430.0f}, {1200.0f, 400.0f}, {1450.0f, 450.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(140.0f, 10.0f)); 
            plat.setFillColor(sf::Color(120, 120, 120)); // Gri platformlar
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Canavar 1: Orijinal Magenta Yatay Devriye Canavarı
        CustomEnemy e1; e1.type = 0; e1.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e1.shape.setFillColor(sf::Color::Magenta); e1.startX = 450.0f; e1.startY = 395.0f; 
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 3.0f; e1.speedY = 0.0f; e1.patrolRange = 100.0f;
        enemies.push_back(e1);

        // Canavar 2: Sonlara doğru eklenen yeni bir Yeşil Yatay Canavar
        CustomEnemy e2; e2.type = 0; e2.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e2.shape.setFillColor(sf::Color::Green); e2.startX = 950.0f; e2.startY = 405.0f; 
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = -2.5f; e2.speedY = 0.0f; e2.patrolRange = 80.0f;
        enemies.push_back(e2);
    } 
    // --- LEVEL 2: Orta Seviye Tırmanış (3 Canavar) ---
    else if (level == 2) {
        std::vector<sf::Vector2f> platPositions = {
            {200.0f, 480.0f}, {380.0f, 390.0f}, {550.0f, 320.0f}, 
            {750.0f, 420.0f}, {980.0f, 350.0f}, {1200.0f, 460.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(120.0f, 12.0f)); 
            plat.setFillColor(sf::Color(100, 149, 237)); // Mavi platformlar
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Canavar 1: Orijinal Turkuaz (Cyan) Uçan Dikey Canavar
        CustomEnemy e1; e1.type = 1; e1.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        e1.shape.setFillColor(sf::Color::Cyan); e1.startX = 480.0f; e1.startY = 250.0f;
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 0.0f; e1.speedY = 2.5f; e1.patrolRange = 80.0f;
        enemies.push_back(e1);

        // Canavar 2: Havada sağa sola hızlı kaçan Mavi bir canavar
        CustomEnemy e2; e2.type = 0; e2.shape.setSize(sf::Vector2f(30.0f, 30.0f));
        e2.shape.setFillColor(sf::Color::Blue); e2.startX = 750.0f; e2.startY = 380.0f;
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = 4.0f; e2.speedY = 0.0f; e2.patrolRange = 100.0f;
        enemies.push_back(e2);

        // Canavar 3: Son platformda bekleyen Sarı bir canavar
        CustomEnemy e3; e3.type = 0; e3.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e3.shape.setFillColor(sf::Color::Yellow); e3.startX = 980.0f; e3.startY = 320.0f;
        e3.shape.setPosition(sf::Vector2f(e3.startX, e3.startY)); e3.speedX = 2.0f; e3.speedY = 0.0f; e3.patrolRange = 60.0f;
        enemies.push_back(e3);
    } 
    // --- LEVEL 3: UZATILMIŞ VE ZORLAŞTIRILMIŞ FİNAL (5 Canavar!) ---
    else if (level == 3) {
        // Platform listesini uzattık, artık yol çok daha uzun!
        std::vector<sf::Vector2f> platPositions = {
            {150.0f, 450.0f}, {320.0f, 360.0f}, {500.0f, 420.0f},
            {700.0f, 350.0f}, {900.0f, 440.0f}, {1100.0f, 380.0f},
            {1300.0f, 460.0f}, {1500.0f, 400.0f} // Ekstra platformlar eklenerek harita uzatıldı
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(110.0f, 15.0f)); 
            plat.setFillColor(sf::Color(178, 34, 34)); // Kırmızı tehlikeli platformlar
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Canavar 1: Orijinal Kırmızı Korkunç Dikey Canavar
        CustomEnemy e1; e1.type = 1; e1.shape.setSize(sf::Vector2f(25.0f, 25.0f));
        e1.shape.setFillColor(sf::Color::Red); e1.startX = 320.0f; e1.startY = 260.0f;
        e1.shape.setPosition(sf::Vector2f(e1.startX, e1.startY)); e1.speedX = 0.0f; e1.speedY = 3.5f; e1.patrolRange = 90.0f;
        enemies.push_back(e1);

        // Canavar 2: İkinci boşlukta aşağı yukarı uçan Sarı bir canavar
        CustomEnemy e2; e2.type = 1; e2.shape.setSize(sf::Vector2f(25.0f, 25.0f));
        e2.shape.setFillColor(sf::Color::Yellow); e2.startX = 700.0f; e2.startY = 220.0f;
        e2.shape.setPosition(sf::Vector2f(e2.startX, e2.startY)); e2.speedX = 0.0f; e2.speedY = 4.0f; e2.patrolRange = 100.0f;
        enemies.push_back(e2);

        // Canavar 3: Uzatılan kısımdaki geniş platformda hızlıca yürüyen Turuncu canavar
        CustomEnemy e3; e3.type = 0; e3.shape.setSize(sf::Vector2f(35.0f, 25.0f));
        e3.shape.setFillColor(sf::Color(255, 165, 0)); e3.startX = 900.0f; e3.startY = 410.0f;
        e3.shape.setPosition(sf::Vector2f(e3.startX, e3.startY)); e3.speedX = 5.0f; e3.speedY = 0.0f; e3.patrolRange = 80.0f;
        enemies.push_back(e3);

        // Canavar 4: Havada asılı duran mermi gibi hızlı git-gel yapan Beyaz bir canavar
        CustomEnemy e4; e4.type = 0; e4.shape.setSize(sf::Vector2f(30.0f, 20.0f));
        e4.shape.setFillColor(sf::Color::White); e4.startX = 1100.0f; e4.startY = 340.0f;
        e4.shape.setPosition(sf::Vector2f(e4.startX, e4.startY)); e4.speedX = -6.0f; e4.speedY = 0.0f; e4.patrolRange = 90.0f;
        enemies.push_back(e4);

        // Canavar 5: Son kapıdan hemen önce duran devasa bir Mor canavar
        CustomEnemy e5; e5.type = 0; e5.shape.setSize(sf::Vector2f(40.0f, 40.0f));
        e5.shape.setFillColor(sf::Color(128, 0, 128)); e5.startX = 1500.0f; e5.startY = 350.0f;
        e5.shape.setPosition(sf::Vector2f(e5.startX, e5.startY)); e5.speedX = 1.5f; e5.speedY = 0.0f; e5.patrolRange = 50.0f;
        enemies.push_back(e5);
    }

    // --- ÖZELLEŞTİRİLMİŞ ALTINLARI PLATFORMLARA SIRALAMA (0.09f BOYUTUYLA) ---
    for (const auto& plat : platforms) {
        GoldCoin coin;
        coin.sprite = new sf::Sprite(goldTexture); 
        coin.sprite->setScale(sf::Vector2f(0.09f, 0.09f)); // İstediğin o şık ideal pikselli altın boyutu
        
        sf::FloatRect coinBounds = coin.sprite->getLocalBounds();
        coin.sprite->setOrigin(sf::Vector2f(coinBounds.size.x / 2.0f, coinBounds.size.y / 2.0f));
        coin.sprite->setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f), plat.getPosition().y - 44.0f));
        
        coin.bounds = coin.sprite->getGlobalBounds(); 
        coin.isCollected = false;
        coins.push_back(coin);
    }

    // Geçiş kapısı her zaman uzayan haritanın en son platformunun ilerisinde nizamlıca belirir
    levelGate.setPosition(sf::Vector2f(platforms.back().getPosition().x + 220.0f, 400.0f));
}

// --- ANA OYUN MOTORU ---
int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Kosucu", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60); 

    sf::View camera(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));

    // Font Yükleme Güvenliği
    sf::Font font;
    if (!font.openFromFile("assets/pixel.ttf")) {
        if (!font.openFromFile("../assets/pixel.ttf")) {
            if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                font.openFromFile("/System/Library/Fonts/Courier.dfont");
            }
        }
    }

    // Arayüz can kalpleri texture yüklemesi
    sf::Texture heartTexture;
    bool hasHeartImg = false;
    if (heartTexture.loadFromFile("assets/pixel_heart.png")) hasHeartImg = true;
    else if (heartTexture.loadFromFile("../assets/pixel_heart.png")) hasHeartImg = true;

    sf::Sprite heartSprite(heartTexture);
    if (hasHeartImg) {
        heartSprite.setScale(sf::Vector2f(0.06f, 0.06f)); 
    }

    // Altın görseli texture yüklemesi
    sf::Texture goldTexture;
    if (!goldTexture.loadFromFile("assets/gold.png")) {
        goldTexture.loadFromFile("../assets/gold.png");
    }

    // Bordomsu koyu pembe renk tonu ve kalın font ayarı
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
    std::vector<GoldCoin> coins; 

    int currentLevel = 1;
    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture);

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

        showWarning = false; 
        // --- SÜRÜM BAĞIMSIZ HATA VERMEYEN OPERATÖR MATEMATİKSEL ÇARPIŞMALARI ---
        sf::FloatRect gateBounds = levelGate.getGlobalBounds();
        if (playerBounds.position.x < gateBounds.position.x + gateBounds.size.x &&
            playerBounds.position.x + playerBounds.size.x > gateBounds.position.x &&
            playerBounds.position.y < gateBounds.position.y + gateBounds.size.y &&
            playerBounds.position.y + playerBounds.size.y > gateBounds.position.y) {
            
            if (score >= 60) { 
                if (currentLevel < 3) {
                    currentLevel++; 
                    score = 0; 
                    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture);
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
        if (playerBounds.position.x < groundBounds.position.x + groundBounds.size.x &&
            playerBounds.position.x + playerBounds.size.x > groundBounds.position.x &&
            playerBounds.position.y < groundBounds.position.y + groundBounds.size.y &&
            playerBounds.position.y + playerBounds.size.y > groundBounds.position.y) {
            
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
            if (playerBounds.position.x < platBounds.position.x + platBounds.size.x &&
                playerBounds.position.x + playerBounds.size.x > platBounds.position.x &&
                playerBounds.position.y < platBounds.position.y + platBounds.size.y &&
                playerBounds.position.y + playerBounds.size.y > platBounds.position.y) {
                
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
            sf::FloatRect bBounds = bullets[i].getGlobalBounds();

            for (size_t j = 0; j < enemies.size(); j++) {
                sf::FloatRect eBounds = enemies[j].shape.getGlobalBounds();
                
                if (bBounds.position.x < eBounds.position.x + eBounds.size.x &&
                    bBounds.position.x + bBounds.size.x > eBounds.position.x &&
                    bBounds.position.y < eBounds.position.y + eBounds.size.y &&
                    bBounds.position.y + bBounds.size.y > eBounds.position.y) {
                    
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
            sf::FloatRect eBounds = enemy.shape.getGlobalBounds();
            if (playerBounds.position.x < eBounds.position.x + eBounds.size.x &&
                playerBounds.position.x + playerBounds.size.x > eBounds.position.x &&
                playerBounds.position.y < eBounds.position.y + eBounds.size.y &&
                playerBounds.position.y + playerBounds.size.y > eBounds.position.y) {
                
                health--; 
                score = 0; 
                generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture); 
                player.resetPosition(sf::Vector2f(100.0f, 400.0f)); 
                bullets.clear();
                if (health <= 0) isGameOver = true; 
                break;
            }
        }

        // Altınları toplama döngüsü
        for (size_t i = 0; i < coins.size(); ) {
            sf::FloatRect cBounds = coins[i].bounds;
            if (!coins[i].isCollected && 
                playerBounds.position.x < cBounds.position.x + cBounds.size.x &&
                playerBounds.position.x + playerBounds.size.x > cBounds.position.x &&
                playerBounds.position.y < cBounds.position.y + cBounds.size.y &&
                playerBounds.position.y + playerBounds.size.y > cBounds.position.y) {
                
                delete coins[i].sprite; 
                coins.erase(coins.begin() + i); 
                score += 10; 
            } else {
                i++;
            }
        }

        // --- RETRO ARAYÜZ METİN YAPISI ---
        std::string uiStr = "LEVEL: " + std::to_string(currentLevel) + "\n" +
                            "SCORE: " + std::to_string(score);
        
        if (showWarning) uiStr += "\n[KAPI ICIN 60 PUAN GEREKLI!]";
        
        uiText.setString(uiStr);
        float uiBaseX = camera.getCenter().x - 380.0f;
        float uiBaseY = 20.0f;
        uiText.setPosition(sf::Vector2f(uiBaseX, uiBaseY));

        // --- EKRANA ÇİZİM AŞAMALARI (RENDER) ---
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
        
        for (const auto& c : coins) {
            if (!c.isCollected) window.draw(*(c.sprite));
        }

        player.draw(window);            
        window.draw(uiText); 

        // --- SCORE ALTINDAKİ RETRO KALP SIRALAMASI ---
        if (hasHeartImg) {
            float startHeartX = uiBaseX;    
            float heartY = uiBaseY + 74.0f; 
            
            for (int i = 0; i < health; i++) {
                heartSprite.setPosition(sf::Vector2f(startHeartX + (i * 38.0f), heartY)); 
                window.draw(heartSprite);
            }
        }
        
        window.display();           
    }

    for (auto& c : coins) {
        delete c.sprite;
    }

    return 0;
}