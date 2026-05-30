#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 
#include <vector> 
#include <cmath>
#include <iostream>
#include <cstdlib> 
#include <ctime>

// --- OYUN NESNELERİNİN YAPILARI (STRUCTS) ---

struct CustomEnemy {
    sf::Sprite* sprite;      
    int type;               // 0: Yatay devriye gezen, 1: Dikey uçan canavar
    float speedX;
    float speedY;
    float startX;
    float startY;
    float patrolRange;
};

struct GoldCoin {
    sf::Sprite* sprite; 
    sf::FloatRect bounds; 
    bool isCollected;   
};

struct Confetti {
    sf::RectangleShape shape;
    float speedX;
    float speedY;
    float rotationSpeed;
};

// --- ORİJİNAL BÖLÜM TASARIM MOTORU (ESKİ LEVELLER) ---
void generateFixedLevel(int level, std::vector<sf::RectangleShape>& platforms, std::vector<CustomEnemy*>& enemies, std::vector<GoldCoin>& coins, sf::Sprite& levelGate, const sf::Texture& goldTexture, const sf::Texture& enemyTexture) {
    
    // Hafıza temizliği
    for (auto& c : coins) delete c.sprite;
    for (auto& e : enemies) {
        if (e && e->sprite) delete e->sprite;
        delete e;
    }
    platforms.clear(); enemies.clear(); coins.clear();

    // --- LEVEL 1 (Orijinal Düzen) ---
    if (level == 1) {
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

        // Düşman 1 (Yatay)
        CustomEnemy* e1 = new CustomEnemy(); e1->type = 0; 
        e1->sprite = new sf::Sprite(enemyTexture);
        e1->sprite->setScale(sf::Vector2f(0.15f, 0.15f)); 
        e1->startX = 450.0f; e1->startY = 360.0f;          
        e1->sprite->setPosition(sf::Vector2f(e1->startX, e1->startY)); 
        e1->speedX = 3.0f; e1->speedY = 0.0f; e1->patrolRange = 100.0f;
        enemies.push_back(e1);

        // Düşman 2 (Yatay)
        CustomEnemy* e2 = new CustomEnemy(); e2->type = 0; 
        e2->sprite = new sf::Sprite(enemyTexture);
        e2->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e2->startX = 950.0f; e2->startY = 370.0f; 
        e2->sprite->setPosition(sf::Vector2f(e2->startX, e2->startY)); 
        e2->speedX = -2.5f; e2->speedY = 0.0f; e2->patrolRange = 80.0f;
        enemies.push_back(e2);
    } 
    // --- LEVEL 2 (Orijinal Düzen) ---
    else if (level == 2) {
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

        // Düşman 1 (Dikey uçan)
        CustomEnemy* e1 = new CustomEnemy(); e1->type = 1; 
        e1->sprite = new sf::Sprite(enemyTexture);
        e1->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e1->startX = 480.0f; e1->startY = 250.0f;
        e1->sprite->setPosition(sf::Vector2f(e1->startX, e1->startY)); 
        e1->speedX = 0.0f; e1->speedY = 2.5f; e1->patrolRange = 80.0f;
        enemies.push_back(e1);

        // Düşman 2 (Yatay)
        CustomEnemy* e2 = new CustomEnemy(); e2->type = 0; 
        e2->sprite = new sf::Sprite(enemyTexture);
        e2->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e2->startX = 750.0f; e2->startY = 360.0f;
        e2->sprite->setPosition(sf::Vector2f(e2->startX, e2->startY)); 
        e2->speedX = 4.0f; e2->speedY = 0.0f; e2->patrolRange = 100.0f;
        enemies.push_back(e2);

        // Düşman 3 (Yatay)
        CustomEnemy* e3 = new CustomEnemy(); e3->type = 0; 
        e3->sprite = new sf::Sprite(enemyTexture);
        e3->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e3->startX = 980.0f; e3->startY = 290.0f;
        e3->sprite->setPosition(sf::Vector2f(e3->startX, e3->startY)); 
        e3->speedX = 2.0f; e3->speedY = 0.0f; e3->patrolRange = 60.0f;
        enemies.push_back(e3);
    } 
    // --- LEVEL 3 (Orijinal Düzen) ---
    else if (level == 3) {
        std::vector<sf::Vector2f> platPositions = {
            {150.0f, 450.0f}, {320.0f, 360.0f}, {500.0f, 420.0f},
            {700.0f, 350.0f}, {900.0f, 440.0f}, {1100.0f, 380.0f},
            {1300.0f, 460.0f}, {1500.0f, 400.0f} 
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(110.0f, 15.0f)); 
            plat.setFillColor(sf::Color(178, 34, 34)); 
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        CustomEnemy* e1 = new CustomEnemy(); e1->type = 1; 
        e1->sprite = new sf::Sprite(enemyTexture);
        e1->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e1->startX = 320.0f; e1->startY = 260.0f;
        e1->sprite->setPosition(sf::Vector2f(e1->startX, e1->startY));
        e1->speedX = 0.0f; e1->speedY = 3.5f; e1->patrolRange = 90.0f;
        enemies.push_back(e1);

        CustomEnemy* e2 = new CustomEnemy(); e2->type = 1; 
        e2->sprite = new sf::Sprite(enemyTexture);
        e2->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e2->startX = 700.0f; e2->startY = 220.0f;
        e2->sprite->setPosition(sf::Vector2f(e2->startX, e2->startY));
        e2->speedX = 0.0f; e2->speedY = 4.0f; e2->patrolRange = 100.0f;
        enemies.push_back(e2);

        CustomEnemy* e3 = new CustomEnemy(); e3->type = 0; 
        e3->sprite = new sf::Sprite(enemyTexture);
        e3->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e3->startX = 900.0f; e3->startY = 380.0f;
        e3->sprite->setPosition(sf::Vector2f(e3->startX, e3->startY));
        e3->speedX = 5.0f; e3->speedY = 0.0f; e3->patrolRange = 80.0f;
        enemies.push_back(e3);

        CustomEnemy* e4 = new CustomEnemy(); e4->type = 0; 
        e4->sprite = new sf::Sprite(enemyTexture);
        e4->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e4->startX = 1100.0f; e4->startY = 320.0f;
        e4->sprite->setPosition(sf::Vector2f(e4->startX, e4->startY));
        e4->speedX = -6.0f; e4->speedY = 0.0f; e4->patrolRange = 90.0f;
        enemies.push_back(e4);

        CustomEnemy* e5 = new CustomEnemy(); e5->type = 0; 
        e5->sprite = new sf::Sprite(enemyTexture);
        e5->sprite->setScale(sf::Vector2f(0.20f, 0.20f)); 
        e5->startX = 1500.0f; e5->startY = 340.0f;
        e5->sprite->setPosition(sf::Vector2f(e5->startX, e5->startY));
        e5->speedX = 1.5f; e5->speedY = 0.0f; e5->patrolRange = 50.0f;
        enemies.push_back(e5);
    }

    // Altın yerleşimi
    for (const auto& plat : platforms) {
        GoldCoin coin;
        coin.sprite = new sf::Sprite(goldTexture); 
        coin.sprite->setScale(sf::Vector2f(0.09f, 0.09f)); 
        sf::FloatRect coinBounds = coin.sprite->getLocalBounds();
        coin.sprite->setOrigin(sf::Vector2f(coinBounds.size.x / 2.0f, coinBounds.size.y / 2.0f));
        coin.sprite->setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f), plat.getPosition().y - 44.0f));
        coin.bounds = coin.sprite->getGlobalBounds(); 
        coin.isCollected = false;
        coins.push_back(coin);
    }

    sf::FloatRect gateBounds = levelGate.getLocalBounds();
    levelGate.setOrigin(sf::Vector2f(gateBounds.size.x / 2.0f, gateBounds.size.y)); 
    levelGate.setPosition(sf::Vector2f(platforms.back().getPosition().x + 280.0f, 550.0f));
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Kosucu", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60); 

    sf::View camera(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));

    sf::Font font;
    if (!font.openFromFile("assets/pixel.ttf")) {
        if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
            font.openFromFile("Courier.dfont");
        }
    }

    sf::Texture heartTexture;
    bool hasHeartImg = false;
    if (heartTexture.loadFromFile("assets/pixel_heart.png")) hasHeartImg = true;
    sf::Sprite heartSprite(heartTexture);
    if (hasHeartImg) heartSprite.setScale(sf::Vector2f(0.06f, 0.06f)); 

    sf::Texture goldTexture; goldTexture.loadFromFile("assets/gold.png");
    sf::Texture enemyTexture; enemyTexture.loadFromFile("assets/evin.png");
    
    sf::Texture cupTexture; cupTexture.loadFromFile("assets/cup.png");
    sf::Sprite cupSprite(cupTexture);
    cupSprite.setScale(sf::Vector2f(0.40f, 0.40f)); 

    sf::Texture flagTexture; flagTexture.loadFromFile("assets/flag.png");
    sf::Sprite levelGate(flagTexture);
    levelGate.setScale(sf::Vector2f(0.14f, 0.14f)); 

    sf::Color deepBordoPink(139, 0, 70); 
    sf::Text uiText(font);
    uiText.setCharacterSize(28); 
    uiText.setFillColor(deepBordoPink);  
    uiText.setStyle(sf::Text::Bold); 

    sf::Text centerText(font);
    centerText.setCharacterSize(42);
    centerText.setFillColor(sf::Color::Red);

    sf::Color darkYellow(204, 153, 0); 
    sf::Text winText1(font);
    winText1.setCharacterSize(46);
    winText1.setFillColor(darkYellow);
    winText1.setStyle(sf::Text::Bold);
    winText1.setString("TEBRIKLER!!");

    sf::Text winText2(font);
    winText2.setCharacterSize(34);
    winText2.setFillColor(darkYellow);
    winText2.setStyle(sf::Text::Bold);
    winText2.setString("oyunu kazandiniz <33");

    sf::Text warningText(font);
    warningText.setCharacterSize(32);           
    warningText.setFillColor(sf::Color(0, 0, 102)); 
    warningText.setString("KAPI ICIN 60 PUAN GEREKLI!");

    sf::RectangleShape ground(sf::Vector2f(5000.0f, 50.0f));
    ground.setFillColor(sf::Color(100, 100, 100));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    sf::RectangleShape softYellowBg(sf::Vector2f(800.0f, 600.0f));
    softYellowBg.setFillColor(sf::Color(255, 255, 204)); 

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
    std::vector<CustomEnemy*> enemies; 
    std::vector<GoldCoin> coins; 
    std::vector<Confetti> confettis;
    
    // Rengarenk şölen için konfeti renk paleti genişletildi kanki
    std::vector<sf::Color> confettiColors = { 
        sf::Color(255,105,180), sf::Color(0,255,255), sf::Color(255,215,0),
        sf::Color(50,205,50), sf::Color(255,69,0), sf::Color(147,112,219)
    };

    int currentLevel = 1;
    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture);

    // Yeni jilet gibi olan ara boyut ayarımız
    Player player(sf::Vector2f(85.0f, 130.0f), sf::Vector2f(100.0f, 350.0f));
    sf::FloatRect playerBounds = player.getBounds();

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
            if (event->is<sf::Event::Closed>()) window.close();
        }

        if (isGameOver) {
            window.setView(window.getDefaultView()); 
            window.clear(sf::Color(45, 45, 45)); 
            centerText.setString("GAME OVER");
            sf::FloatRect textBounds = centerText.getLocalBounds();
            centerText.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
            centerText.setPosition(sf::Vector2f(400.0f, 200.0f)); 
            window.draw(centerText);
            window.display();
            continue; 
        }

        // --- ORİJİNAL "YOU WIN" EKRANI MOTORU ---
        if (isGameWon) {
            // Kamerayı oyun alanı karmaşasından sıfırlayıp tam merkeze çekiyoruz (Ekran düzeldi!)
            window.setView(window.getDefaultView()); 
            
            if (confettis.size() < 150) {
                Confetti c; 
                c.shape.setSize(sf::Vector2f(9.0f, 9.0f)); 
                c.shape.setFillColor(confettiColors[std::rand() % confettiColors.size()]);
                c.shape.setOrigin(sf::Vector2f(4.5f, 4.5f));
                c.shape.setPosition(sf::Vector2f(static_cast<float>(std::rand() % 800), -20.0f));
                c.speedX = static_cast<float>((std::rand() % 40) - 20) / 10.0f; 
                c.speedY = static_cast<float>((std::rand() % 30) + 20) / 10.0f;  
                c.rotationSpeed = static_cast<float>((std::rand() % 10) - 5);    
                confettis.push_back(c);
            }

            for (auto& c : confettis) {
                c.shape.move(sf::Vector2f(c.speedX, c.speedY));
                c.shape.rotate(sf::Angle(sf::degrees(c.rotationSpeed)));
                if (c.shape.getPosition().y > 600.0f) {
                    c.shape.setPosition(sf::Vector2f(static_cast<float>(std::rand() % 800), -10.0f));
                    c.speedY = static_cast<float>((std::rand() % 30) + 20) / 10.0f;
                }
            }

            window.clear(); 
            window.draw(softYellowBg);

            sf::FloatRect bounds1 = winText1.getLocalBounds();
            winText1.setOrigin(sf::Vector2f(bounds1.size.x / 2.0f, bounds1.size.y / 2.0f));
            winText1.setPosition(sf::Vector2f(400.0f, 110.0f));

            sf::FloatRect bounds2 = winText2.getLocalBounds();
            winText2.setOrigin(sf::Vector2f(bounds2.size.x / 2.0f, bounds2.size.y / 2.0f));
            winText2.setPosition(sf::Vector2f(400.0f, 180.0f));

            sf::FloatRect cupBounds = cupSprite.getLocalBounds();
            cupSprite.setOrigin(sf::Vector2f(cupBounds.size.x / 2.0f, cupBounds.size.y / 2.0f));
            cupSprite.setPosition(sf::Vector2f(400.0f, 390.0f));

            window.draw(winText1); 
            window.draw(winText2); 
            window.draw(cupSprite);
            
            for (const auto& c : confettis) window.draw(c.shape);
            window.display(); 
            continue;
        }

        bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
        bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
        bool isFiring = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
        bool jumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

        sf::Vector2f currentPos = player.getBounds().position;
        if (moveLeft) currentPos.x -= 5.0f;
        if (moveRight) currentPos.x += 5.0f;

        if (isFiring && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
            sf::CircleShape newBullet(5.0f);
            newBullet.setFillColor(sf::Color::Yellow); 
            newBullet.setPosition(sf::Vector2f(currentPos.x + player.getBounds().size.x, currentPos.y + 40.0f));
            bullets.push_back(newBullet);
            shootClock.restart();
        }

        if (platformJumpVelocity < 0.0f) {
            currentPos.y += platformJumpVelocity; platformJumpVelocity += 0.6f; 
        } else {
            currentPos.y += 4.5f; 
        }

        player.resetPosition(currentPos);
        player.update(moveLeft, moveRight, isFiring);
        playerBounds = player.getBounds(); 

        float playerX = playerBounds.position.x;
        if (playerX > 400.0f) camera.setCenter(sf::Vector2f(playerX, 300.0f));
        else camera.setCenter(sf::Vector2f(400.0f, 300.0f));
        window.setView(camera); 

        if (hasBg && bgWidth > 0.0f) {
            float bgX = camera.getCenter().x * 0.2f; float offset = std::fmod(bgX, bgWidth);
            bgSprite1.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset, 0.0f));
            bgSprite2.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset + bgWidth, 0.0f));
        }

        for (auto& enemy : enemies) {
            if (enemy && enemy->sprite) {
                if (enemy->type == 0) { 
                    enemy->sprite->move(sf::Vector2f(enemy->speedX, 0.0f));
                    if (std::abs(enemy->sprite->getPosition().x - enemy->startX) > enemy->patrolRange) enemy->speedX *= -1.0f;
                } else if (enemy->type == 1) { 
                    enemy->sprite->move(sf::Vector2f(0.0f, enemy->speedY));
                    if (std::abs(enemy->sprite->getPosition().y - enemy->startY) > enemy->patrolRange) enemy->speedY *= -1.0f;
                }
            }
        }

        showWarning = false; sf::FloatRect gateBounds = levelGate.getGlobalBounds();
        if (playerBounds.position.x < gateBounds.position.x + gateBounds.size.x && playerBounds.position.x + playerBounds.size.x > gateBounds.position.x && playerBounds.position.y < gateBounds.position.y + gateBounds.size.y && playerBounds.position.y + playerBounds.size.y > gateBounds.position.y) {
            if (score >= 60) { 
                if (currentLevel < 3) {
                    currentLevel++; score = 0; 
                    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture);
                    player.resetPosition(sf::Vector2f(100.0f, 350.0f)); platformJumpVelocity = 0.0f; bullets.clear();
                } else isGameWon = true;
            } else showWarning = true;
        }

        sf::FloatRect groundBounds = ground.getGlobalBounds();
        if (playerBounds.position.x < groundBounds.position.x + groundBounds.size.x && playerBounds.position.x + playerBounds.size.x > groundBounds.position.x && playerBounds.position.y < groundBounds.position.y + groundBounds.size.y && playerBounds.position.y + playerBounds.size.y > groundBounds.position.y) {
            if (playerBounds.position.y + playerBounds.size.y <= groundBounds.position.y + 14.0f) {
                player.resetPosition(sf::Vector2f(playerBounds.position.x, groundBounds.position.y - playerBounds.size.y));
                playerBounds = player.getBounds(); 
                if (jumpKeyPressed) platformJumpVelocity = -14.5f; 
                else if (platformJumpVelocity > 0.0f) platformJumpVelocity = 0.0f;
            }
        }

        for (const auto& platform : platforms) {
            sf::FloatRect platBounds = platform.getGlobalBounds();
            if (playerBounds.position.x < platBounds.position.x + platBounds.size.x && playerBounds.position.x + playerBounds.size.x > platBounds.position.x && playerBounds.position.y < platBounds.position.y + platBounds.size.y && playerBounds.position.y + playerBounds.size.y > platBounds.position.y) {
                if (playerBounds.position.y + playerBounds.size.y <= platBounds.position.y + 14.0f) {
                    player.resetPosition(sf::Vector2f(playerBounds.position.x, platBounds.position.y - playerBounds.size.y));
                    playerBounds = player.getBounds(); 
                    if (jumpKeyPressed) platformJumpVelocity = -14.5f; 
                    else if (platformJumpVelocity > 0.0f) platformJumpVelocity = 0.0f;
                }
            }
        }

        for (size_t i = 0; i < bullets.size(); ) {
            bullets[i].move(sf::Vector2f(8.0f, 0.0f)); bool bulletRemoved = false;
            sf::FloatRect bBounds = bullets[i].getGlobalBounds();
            for (size_t j = 0; j < enemies.size(); j++) {
                if (enemies[j] && enemies[j]->sprite) {
                    sf::FloatRect eBounds = enemies[j]->sprite->getGlobalBounds();
                    if (bBounds.position.x < eBounds.position.x + eBounds.size.x && bBounds.position.x + bBounds.size.x > eBounds.position.x && bBounds.position.y < eBounds.position.y + eBounds.size.y && bBounds.position.y + bBounds.size.y > eBounds.position.y) {
                        delete enemies[j]->sprite; delete enemies[j]; enemies.erase(enemies.begin() + j); bullets.erase(bullets.begin() + i); score += 20; bulletRemoved = true; break;
                    }
                }
            }
            if (!bulletRemoved) {
                if (bullets[i].getPosition().x > camera.getCenter().x + 400.0f) bullets.erase(bullets.begin() + i);
                else i++;
            }
        }

        for (const auto& enemy : enemies) {
            if (enemy && enemy->sprite) {
                sf::FloatRect eBounds = enemy->sprite->getGlobalBounds();
                if (playerBounds.position.x < eBounds.position.x + eBounds.size.x && playerBounds.position.x + playerBounds.size.x > eBounds.position.x && playerBounds.position.y < eBounds.position.y + eBounds.size.y && playerBounds.position.y + playerBounds.size.y > eBounds.position.y) {
                    health--; score = 0; generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture); 
                    player.resetPosition(sf::Vector2f(100.0f, 350.0f)); bullets.clear();
                    if (health <= 0) isGameOver = true; break;
                }
            }
        }

        for (size_t i = 0; i < coins.size(); ) {
            sf::FloatRect cBounds = coins[i].bounds;
            if (!coins[i].isCollected && playerBounds.position.x < cBounds.position.x + cBounds.size.x && playerBounds.position.x + playerBounds.size.x > cBounds.position.x && playerBounds.position.y < cBounds.position.y + cBounds.size.y && playerBounds.position.y + playerBounds.size.y > cBounds.position.y) { 
                delete coins[i].sprite; coins.erase(coins.begin() + i); score += 10; 
            } else i++;
        }

        std::string uiStr = "LEVEL: " + std::to_string(currentLevel) + "\n" + "SCORE: " + std::to_string(score);
        uiText.setString(uiStr); uiText.setPosition(sf::Vector2f(camera.getCenter().x - 380.0f, 20.0f));

        window.clear(sf::Color::Black); 
        if (hasBg && bgWidth > 0.0f) { window.draw(bgSprite1); window.draw(bgSprite2); }   
        window.draw(ground); window.draw(levelGate); 
        for (const auto& enemy : enemies) if (enemy && enemy->sprite) window.draw(*(enemy->sprite)); 
        for (const auto& b : bullets) window.draw(b);
        for (const auto& platform : platforms) window.draw(platform);
        for (const auto& c : coins) if (!c.isCollected) window.draw(*(c.sprite));
        player.draw(window); window.draw(uiText); 

        if (showWarning) {
            warningText.setPosition(sf::Vector2f(camera.getCenter().x - 240.0f, 260.0f)); window.draw(warningText);
        }
        if (hasHeartImg) {
            for (int i = 0; i < health; i++) {
                heartSprite.setPosition(sf::Vector2f((camera.getCenter().x - 380.0f) + (i * 38.0f), 94.0f)); window.draw(heartSprite);
            }
        }
        window.display();           
    } 

    for (auto& c : coins) delete c.sprite;
    for (auto& e : enemies) { if (e && e->sprite) delete e->sprite; delete e; }
    return 0;
}