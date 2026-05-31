#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Ses motoru, arka plan müzikleri ve anlık ses efektleri için kütüphanemiz
#include "Player.hpp"
#include <string> 
#include <vector> 
#include <cmath>
#include <iostream>
#include <cstdlib> 
#include <ctime>

// --- MAC OS DİZİN AYARI ---
// Mac kullananlar için assets klasörünün yerini otomatik/\ bulan fonksiyonum.
#include <filesystem>
std::string getResourcePath(const std::string& filename) {
    if (std::filesystem::exists("assets/" + filename)) {
        return "assets/" + filename;
    }
    if (std::filesystem::exists("../assets/" + filename)) {
        return "../assets/" + filename;
    }
    return "assets/" + filename; 
}

// --- OYUN NESNELERİNİN YAPILARI (STRUCTS) ---

// Haritaya ekleyeceğim düşmanların yapısal özelliklerini burada topladım.
struct CustomEnemy {
    sf::Sprite* sprite;      // Düşmanın ekrandaki görseli
    int type;                // 0 ise sağ-sol devriyesi, 1 ise yukarı-aşağı devriyesi
    float speedX;            // X eksenindeki hız vektörü
    float speedY;            // Y eksenindeki hız vektörü
    float startX;            // İlk doğduğu X koordinatı (Devriye sınırı için)
    float startY;            // İlk doğduğu Y koordinatı
    float patrolRange;       // Doğduğu yerden ne kadar uzağa gidip döneceği (Menzil)
};

// Toplanabilir altınların yapısı
struct GoldCoin {
    sf::Sprite* sprite;      // Altının görseli
    sf::FloatRect bounds;    // Çarpışma kutusu (Karakterin temasını yakalamak için)
    bool isCollected;        // Altın toplandı mı toplanmadı mı kontrolü
};

// Oyun kazanıldığında ekrandan aşağı dökülecek konfetilerin yapısı
struct Confetti {
    sf::RectangleShape shape; // Küçük renkli kareler
    float speedX;            // Sağa sola savrulma hızı
    float speedY;            // Aşağı düşme hızı
    float rotationSpeed;     // Kendi etrafında dönme hızı
};

// --- OYUN AKIŞ DURUMLARI (STATE MACHINE) ---
// Oyunun hangi ekranda olduğunu yönetmek için enum yapısı kurdum.
enum class GameState {
    START_MENU, // İlk açılıştaki ana menu ekranı
    GAMEPLAY    // Aktif olarak oynadığımız oyun ekranı
};

// --- BÖLÜM TASARIM MOTORU (LEVEL GENERATOR) ---
// Seviyeleri milimetrik olarak tasarlamış, platformları ve nesneleri dizen fonksiyonum.
void generateFixedLevel(int level, std::vector<sf::RectangleShape>& platforms, std::vector<CustomEnemy*>& enemies, std::vector<GoldCoin>& coins, sf::Sprite& levelGate, const sf::Texture& goldTexture, const sf::Texture& enemyTexture, const sf::Texture& blockTexture) {
    
    // Yeni levele geçerken eski levelden kalan dinamik bellekleri temizliyorum (Memory Leak önlemi).
    for (auto& c : coins) delete c.sprite;
    for (auto& e : enemies) {
        if (e && e->sprite) delete e->sprite;
        delete e;
    }
    platforms.clear(); enemies.clear(); coins.clear();

    // --- BÖLÜM 1 TASARIMI ---
    if (level == 1) {
        // Havada asılı duracak platformların koordinatlarını belirliyorum
        std::vector<sf::Vector2f> platPositions = {
            {250.0f, 420.0f}, {450.0f, 390.0f}, {700.0f, 430.0f}, 
            {950.0f, 400.0f}, {1200.0f, 370.0f}, {1450.0f, 420.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(140.0f, 24.0f)); 
            plat.setPosition(pos);
            plat.setTexture(&blockTexture); // Platform dokusunu giydiriyorum
            platforms.push_back(plat);
        }

        // İlk düşmanı yatay devriye (type = 0) olarak haritaya ekliyorum
        CustomEnemy* e1 = new CustomEnemy(); e1->type = 0; 
        e1->sprite = new sf::Sprite(enemyTexture);
        e1->sprite->setScale(sf::Vector2f(0.15f, 0.15f)); 
        e1->startX = 450.0f; e1->startY = 350.0f;          
        e1->sprite->setPosition(sf::Vector2f(e1->startX, e1->startY)); 
        e1->speedX = 3.0f; e1->speedY = 0.0f; e1->patrolRange = 100.0f;
        enemies.push_back(e1);

        // İkinci düşmanı da ters yönde hareket edecek şekilde kuruyorum
        CustomEnemy* e2 = new CustomEnemy(); e2->type = 0; 
        e2->sprite = new sf::Sprite(enemyTexture);
        e2->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e2->startX = 950.0f; e2->startY = 360.0f; 
        e2->sprite->setPosition(sf::Vector2f(e2->startX, e2->startY)); 
        e2->speedX = -2.5f; e2->speedY = 0.0f; e2->patrolRange = 80.0f;
        enemies.push_back(e2);
    } 
    // --- BÖLÜM 2 TASARIMI ---
    else if (level == 2) {
        std::vector<sf::Vector2f> platPositions = {
            {200.0f, 450.0f}, {380.0f, 360.0f}, {550.0f, 290.0f}, 
            {750.0f, 390.0f}, {980.0f, 320.0f}, {1200.0f, 430.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(120.0f, 24.0f)); 
            plat.setPosition(pos);
            plat.setTexture(&blockTexture);
            platforms.push_back(plat);
        }

        // Level 2'ye yukarı-aşağı uçan dikey bir düşman (type = 1) ekleyerek zorluğu artırıyorum
        CustomEnemy* e1 = new CustomEnemy(); e1->type = 1; 
        e1->sprite = new sf::Sprite(enemyTexture);
        e1->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e1->startX = 480.0f; e1->startY = 250.0f;
        e1->sprite->setPosition(sf::Vector2f(e1->startX, e1->startY)); 
        e1->speedX = 0.0f; e1->speedY = 2.5f; e1->patrolRange = 80.0f;
        enemies.push_back(e1);

        CustomEnemy* e2 = new CustomEnemy(); e2->type = 0; 
        e2->sprite = new sf::Sprite(enemyTexture);
        e2->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e2->startX = 750.0f; e2->startY = 350.0f;
        e2->sprite->setPosition(sf::Vector2f(e2->startX, e2->startY)); 
        e2->speedX = 4.0f; e2->speedY = 0.0f; e2->patrolRange = 100.0f;
        enemies.push_back(e2);

        CustomEnemy* e3 = new CustomEnemy(); e3->type = 0; 
        e3->sprite = new sf::Sprite(enemyTexture);
        e3->sprite->setScale(sf::Vector2f(0.15f, 0.15f));
        e3->startX = 980.0f; e3->startY = 290.0f;
        e3->sprite->setPosition(sf::Vector2f(e3->startX, e3->startY)); 
        e3->speedX = 2.0f; e3->speedY = 0.0f; e3->patrolRange = 60.0f;
        enemies.push_back(e3);
    } 
    // --- BÖLÜM 3 TASARIMI ---
    else if (level == 3) {
        std::vector<sf::Vector2f> platPositions = {
            {150.0f, 420.0f}, {320.0f, 330.0f}, {500.0f, 390.0f},
            {700.0f, 320.0f}, {900.0f, 410.0f}, {1100.0f, 350.0f},
            {1300.0f, 430.0f}, {1500.0f, 370.0f} 
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(110.0f, 24.0f)); 
            plat.setPosition(pos);
            plat.setTexture(&blockTexture);
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

    for (const auto& plat : platforms) {
        GoldCoin coin;
        coin.sprite = new sf::Sprite(goldTexture); 
        coin.sprite->setScale(sf::Vector2f(0.09f, 0.09f)); 
        sf::FloatRect coinBounds = coin.sprite->getLocalBounds();
        coin.sprite->setOrigin(sf::Vector2f(coinBounds.size.x / 2.0f, coinBounds.size.y / 2.0f));
        coin.sprite->setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f), plat.getPosition().y - 30.0f));
        coin.bounds = coin.sprite->getGlobalBounds(); 
        coin.isCollected = false;
        coins.push_back(coin);
    }

    // BAYRAK HİZALAMASI: Şeffaf boşluğu yok etmek için bayrağı çimen yüzeyine tam oturtuyoruz
    sf::FloatRect gateBounds = levelGate.getLocalBounds();
    levelGate.setOrigin(sf::Vector2f(gateBounds.size.x / 2.0f, gateBounds.size.y)); 
    levelGate.setPosition(sf::Vector2f(platforms.back().getPosition().x + 280.0f, 555.0f)); 
}

// --- ANA FONKSİYON (MAIN) ---
int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Platform Kosucu", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60); 

    // --- DEĞİŞKEN TANIMLAMALARI ---
    GameState currentState = GameState::START_MENU;
    int currentLevel = 1;
    int score = 0;
    int health = 5; 
    bool isGameOver = false; 
    bool isGameWon = false;
    bool showWarning = false;
    float platformJumpVelocity = 0.0f; 
    bool playedLoseSound = false;
    bool playedWinSound = false;

    // Sabit 800x600 oyun içi kamera
    sf::View camera(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));
    // Sabit 800x600 arayüz ve menü görünümü (kaymayı önlemek için)
    sf::View uiView(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));

    // --- ARKA PLAN MÜZİĞİ SİSTEMİ ---
    sf::Music gameMusic;
    bool hasMusic = false;
    
    if (gameMusic.openFromFile(getResourcePath("music.ogg"))) {
        hasMusic = true;
    } else if (gameMusic.openFromFile(getResourcePath("music.wav"))) {
        hasMusic = true;
    }

    if (hasMusic) {
        gameMusic.setLooping(true); 
        gameMusic.setVolume(40.0f); 
        gameMusic.play();           
    } else {
        std::cout << "BILGI: Muzik dosyasi bulunamadi veya yuklenemedi, oyun sessiz modda baslatiliyor." << std::endl;
    }

    // --- ANLIK SES EFEKTLERİ MOTORU GÜVENLİ KONTROLÜ ---
    sf::SoundBuffer jumpBuffer, coinBuffer, hitBuffer, winBuffer, loseBuffer;
    bool hasJump = jumpBuffer.loadFromFile(getResourcePath("jump.wav"));
    bool hasCoin = coinBuffer.loadFromFile(getResourcePath("coin.wav"));
    bool hasHit = hitBuffer.loadFromFile(getResourcePath("hit.wav"));
    bool hasWin = winBuffer.loadFromFile(getResourcePath("win.wav"));
    bool hasLose = loseBuffer.loadFromFile(getResourcePath("lose.wav"));

    // Ses nesnelerini tanımlarken buffer'ları doğrudan constructor'a veriyoruz
    sf::Sound jumpSound(jumpBuffer);
    sf::Sound coinSound(coinBuffer);
    sf::Sound hitSound(hitBuffer);
    sf::Sound winSound(winBuffer);
    sf::Sound loseSound(loseBuffer);

    // Ses düzeylerini ayarlıyoruz
    if (hasJump) jumpSound.setVolume(50.0f);
    if (hasCoin) coinSound.setVolume(60.0f);
    if (hasHit)  hitSound.setVolume(50.0f);
    if (hasWin)  winSound.setVolume(60.0f);
    if (hasLose) loseSound.setVolume(60.0f);

    // --- FONT VE YAZI TİPİ AYARLARI ---
    sf::Font font;
    if (!font.openFromFile(getResourcePath("pixel.ttf"))) {
        if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
            (void)font.openFromFile("Courier.dfont"); 
        }
    }
       
    // --- BAŞLANGIÇ EKRANI METİNLERİ VE RENKLERİ ---
    sf::Color lacivert(0, 0, 102);
    sf::Color koyuPembe(219, 112, 147);
    sf::Color acikPembe(255, 192, 203);

    sf::Text menuTitle(font);
    menuTitle.setCharacterSize(64);
    menuTitle.setFillColor(lacivert);
    menuTitle.setStyle(sf::Text::Bold);
    menuTitle.setString("VESPERA");
    sf::FloatRect mtBounds = menuTitle.getLocalBounds();
    menuTitle.setOrigin(sf::Vector2f(mtBounds.size.x / 2.0f, mtBounds.size.y / 2.0f));
    menuTitle.setPosition(sf::Vector2f(400.0f, 220.0f));

    sf::Text menuSub(font);
    menuSub.setCharacterSize(30);
    menuSub.setFillColor(koyuPembe);
    menuSub.setStyle(sf::Text::Bold);
    menuSub.setString("baslamak icin R ye basin!");
    sf::FloatRect msBounds = menuSub.getLocalBounds();
    menuSub.setOrigin(sf::Vector2f(msBounds.size.x / 2.0f, msBounds.size.y / 2.0f));
    menuSub.setPosition(sf::Vector2f(400.0f, 340.0f));

    // --- GÖRSEL (TEXTURE) YÜKLEMELERİ ---
    sf::Texture goldTexture, enemyTexture, cupTexture, flagTexture, blockTexture, groundTexture, gameOverTexture, heartTexture;

    (void)goldTexture.loadFromFile(getResourcePath("gold.png"));
    (void)enemyTexture.loadFromFile(getResourcePath("evin.png"));
    (void)cupTexture.loadFromFile(getResourcePath("cup.png"));
    (void)flagTexture.loadFromFile(getResourcePath("flag.png"));
    (void)blockTexture.loadFromFile(getResourcePath("blok.png"));
    (void)gameOverTexture.loadFromFile(getResourcePath("game_over.png"));
    
    if (!groundTexture.loadFromFile(getResourcePath("grass_block.png"))) {
        std::cout << "HATA: Zemin yuklenemedi!" << std::endl;
    }

    bool hasHeartImg = heartTexture.loadFromFile(getResourcePath("pixel_heart.png"));

    // --- SPRITE AYARLARI ---
    sf::Sprite cupSprite(cupTexture);
    cupSprite.setScale(sf::Vector2f(0.40f, 0.40f));

    sf::Sprite levelGate(flagTexture);
    levelGate.setScale(sf::Vector2f(0.14f, 0.14f));

    sf::Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setScale(sf::Vector2f(0.4f, 0.4f)); 

    sf::Sprite heartSprite(heartTexture);
    if (hasHeartImg) {
        heartSprite.setScale(sf::Vector2f(0.06f, 0.06f));
    }

    // --- SIFIR BOŞLUKLU ULTRA SIKI ZEMİN MOTORU ---
    std::vector<sf::Sprite> groundTiles;
    float currentScale = 0.35f;
    float scaledWidth = 98.0f; 

    int tileCount = static_cast<int>(12000.0f / scaledWidth) + 5; 
    for (int i = 0; i < tileCount; ++i) {
        sf::Sprite tile(groundTexture);
        tile.setScale(sf::Vector2f(currentScale, currentScale)); 
        tile.setPosition(sf::Vector2f(static_cast<float>(i) * scaledWidth, 530.0f)); 
        groundTiles.push_back(tile);
    }

    // --- ARAYÜZ (UI) METİNLERİ ---
    sf::Color deepBordoPink(139, 0, 70); 
    sf::Text uiText(font);
    uiText.setCharacterSize(28); 
    uiText.setStyle(sf::Text::Bold); 

    sf::Text centerText(font);
    centerText.setCharacterSize(64); 
    centerText.setFillColor(sf::Color::White); 
    centerText.setStyle(sf::Text::Bold);
    centerText.setString("GAME OVER");

    sf::Text gameOverRestartText(font);
    gameOverRestartText.setCharacterSize(26);
    gameOverRestartText.setFillColor(sf::Color::White);
    gameOverRestartText.setStyle(sf::Text::Regular);
    gameOverRestartText.setString("tekrar baslamak icin R ye basin");

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

    // --- ARKA PLAN VE PARALLAX KODLARI ---
    sf::RectangleShape softYellowBg(sf::Vector2f(800.0f, 600.0f));
    softYellowBg.setFillColor(sf::Color(255, 255, 204)); 

    sf::Texture bgTexture;
    bool hasBg = bgTexture.loadFromFile(getResourcePath("sunset_cloud.png"));
    sf::Sprite bgSprite1(bgTexture);
    sf::Sprite bgSprite2(bgTexture);
    
    float bgWidth = 0.0f;
    if (hasBg) {
        float bgScaleY = 600.0f / bgTexture.getSize().y;
        bgSprite1.setScale(sf::Vector2f(bgScaleY, bgScaleY)); 
        bgSprite2.setScale(sf::Vector2f(bgScaleY, bgScaleY)); 
        bgWidth = bgTexture.getSize().x * bgScaleY;
    }

    // --- DİNAMİK NESNE VEKTÖRLERİ ---
    std::vector<sf::RectangleShape> platforms;
    std::vector<CustomEnemy*> enemies; 
    std::vector<GoldCoin> coins; 
    std::vector<Confetti> confettis;
    std::vector<sf::Color> confettiColors = { 
        sf::Color(255,105,180), sf::Color(0,255,255), sf::Color(255,215,0),
        sf::Color(50,205,50), sf::Color(255,69,0)
    };

    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture, blockTexture);

    Player player(sf::Vector2f(85.0f, 130.0f), sf::Vector2f(100.0f, 350.0f));
    sf::FloatRect playerBounds = player.getBounds();

    std::vector<sf::CircleShape> bullets; 
    sf::Clock shootClock;         
    float shootCooldown = 0.25f;  

    // --- ANA OYUN DÖNGÜSÜ ---
    while (window.isOpen()) {
        
        // SFML 3.0 UYUMLU MODERN EVENT DÖNGÜSÜ
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // ESC Tuşu ile Tam Ekran / Pencere Modu Geçişi (Ekran Kayması Tamamen Çözüldü!)
            if (event->is<sf::Event::KeyPressed>()) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        static bool isFullscreen = false;
                        isFullscreen = !isFullscreen;

                        if (isFullscreen) {
                            window.create(sf::VideoMode({1920, 1080}), "Platform Runner", sf::Style::None);
                        } else {
                            window.create(sf::VideoMode({800, 600}), "Platform Runner", sf::Style::Default);
                        }
                        
                        // Ekran oranını koruyarak kaymayı engelliyoruz
                        camera.setSize(sf::Vector2f(800.0f, 600.0f));
                        uiView.setSize(sf::Vector2f(800.0f, 600.0f));
                        window.setView(camera);
                        
                        window.setFramerateLimit(60); 
                        window.requestFocus();        
                    }
                }
            }
        }

        // --- 1. DURUM: BAŞLANGIÇ MENÜSÜ MANTIĞI ---
        if (currentState == GameState::START_MENU) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                currentState = GameState::GAMEPLAY;
                if (hasMusic) gameMusic.play();
                playedLoseSound = false;
                playedWinSound = false;
            }

            // Menünün tam ortada sabit kalması için uiView atıyoruz
            window.setView(uiView);
            window.clear(acikPembe); 

            window.draw(menuTitle);  
            window.draw(menuSub);    
            window.display();
            continue; 
        }

        // --- 2. DURUM: OYUN İÇİ (GAMEPLAY) MANTIĞI ---
        
        // --- GAME OVER EKRANI ---
        if (isGameOver) {
            if (hasMusic) gameMusic.stop();
            if (hasLose && !playedLoseSound) {
                loseSound.play();
                playedLoseSound = true;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                isGameOver = false;
                health = 5;
                score = 0;
                currentLevel = 1;
                generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture, blockTexture);
                player.resetPosition(sf::Vector2f(100.0f, 350.0f));
                bullets.clear();
                currentState = GameState::START_MENU; 
                continue;
            }

            // Kedinin köşeye kaçmasını önlemek için sabit uiView verdik!
            window.setView(uiView); 
            window.clear(sf::Color::Black); 

            sf::FloatRect goBounds = gameOverSprite.getLocalBounds();
            gameOverSprite.setOrigin(sf::Vector2f(goBounds.size.x / 2.0f, goBounds.size.y / 2.0f));
            gameOverSprite.setPosition(sf::Vector2f(400.0f, 200.0f)); 

            sf::FloatRect textBounds = centerText.getLocalBounds();
            centerText.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
            centerText.setPosition(sf::Vector2f(400.0f, 400.0f)); 

            sf::FloatRect gorBounds = gameOverRestartText.getLocalBounds();
            gameOverRestartText.setOrigin(sf::Vector2f(gorBounds.size.x / 2.0f, gorBounds.size.y / 2.0f));
            gameOverRestartText.setPosition(sf::Vector2f(400.0f, 480.0f));
            
            window.draw(gameOverSprite);
            window.draw(centerText);
            window.draw(gameOverRestartText); 
            window.display();
            continue; 
        }

        // --- KAZANMA EKRANI ---
        if (isGameWon) {
            if (hasMusic) gameMusic.stop();
            if (hasWin && !playedWinSound) {
                winSound.play();
                playedWinSound = true;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                isGameWon = false;
                health = 5;
                score = 0;
                currentLevel = 1;
                generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture, blockTexture);
                player.resetPosition(sf::Vector2f(100.0f, 350.0f));
                bullets.clear();
                currentState = GameState::START_MENU;
                continue;
            }

            window.setView(uiView); 
            if (confettis.size() < 150) {
                Confetti c; c.shape.setSize(sf::Vector2f(9.0f, 9.0f)); 
                c.shape.setFillColor(confettiColors[std::rand() % confettiColors.size()]);
                c.shape.setPosition(sf::Vector2f(static_cast<float>(std::rand() % 800), -20.0f));
                c.speedX = static_cast<float>((std::rand() % 40) - 20) / 10.0f; 
                c.speedY = static_cast<float>((std::rand() % 30) + 20) / 10.0f;  
                confettis.push_back(c);
            }
            for (auto& c : confettis) {
                c.shape.move(sf::Vector2f(c.speedX, c.speedY));
                if (c.shape.getPosition().y > 600.0f) c.shape.setPosition(sf::Vector2f(static_cast<float>(std::rand() % 800), -10.0f));
            }
       
            window.clear(sf::Color(255, 255, 204)); 
            
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

        // --- GİRDİLER VE KLAVYE KONTROLLERİ ---
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
            newBullet.setPosition(sf::Vector2f(currentPos.x + player.getBounds().size.x, currentPos.y - 40.0f));
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

        // --- KAMERA MERKEZLEME ---
        float playerX = playerBounds.position.x;
        if (playerX > 400.0f) camera.setCenter(sf::Vector2f(playerX, 300.0f));
        else camera.setCenter(sf::Vector2f(400.0f, 300.0f));
        window.setView(camera); 

        if (hasBg && bgWidth > 0.0f) {
            float bgX = camera.getCenter().x * 0.2f; float offset = std::fmod(bgX, bgWidth);
            bgSprite1.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset, 0.0f));
            bgSprite2.setPosition(sf::Vector2f(camera.getCenter().x - 400.0f - offset + bgWidth, 0.0f));
        }

        // --- DÜŞMAN YAPAY ZEKASI ---
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

        // --- BÖLÜM GEÇİŞ VE KAPI KONTROLÜ ---
        showWarning = false; sf::FloatRect gateBounds = levelGate.getGlobalBounds();
        if (playerBounds.position.x - (playerBounds.size.x / 2.0f) < gateBounds.position.x + gateBounds.size.x && 
            playerBounds.position.x + (playerBounds.size.x / 2.0f) > gateBounds.position.x && 
            playerBounds.position.y - playerBounds.size.y < gateBounds.position.y + gateBounds.size.y && 
            playerBounds.position.y > gateBounds.position.y) {
            
            if (score >= 60) { 
                if (currentLevel < 3) {
                    currentLevel++; score = 0; 
                    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture, blockTexture);
                    player.resetPosition(sf::Vector2f(100.0f, 350.0f)); 
                    platformJumpVelocity = 0.0f; 
                    bullets.clear();
                    continue; 
                } else isGameWon = true; 
            } else showWarning = true; 
        }

        // --- MİLİMETRİK ÇİMEN ZEMİN ÇARPIŞMA VE OFFSET KİLİTLEMESİ ---
        for (const auto& tile : groundTiles) {
            sf::FloatRect tileBounds = tile.getGlobalBounds();
            if (playerBounds.position.x - (playerBounds.size.x / 2.0f) < tileBounds.position.x + tileBounds.size.x && 
                playerBounds.position.x + (playerBounds.size.x / 2.0f) > tileBounds.position.x && 
                playerBounds.position.y <= tileBounds.position.y + 35.0f && 
                playerBounds.position.y >= tileBounds.position.y - 10.0f) {
                
                if (platformJumpVelocity >= 0.0f) {
                    player.resetPosition(sf::Vector2f(playerBounds.position.x, tileBounds.position.y + 25.0f));
                    playerBounds = player.getBounds(); 
                    if (jumpKeyPressed) {
                        platformJumpVelocity = -14.5f; 
                        if (hasJump) jumpSound.play(); 
                    }
                    else platformJumpVelocity = 0.0f;
                }
            }
        }

        // --- ASILI PLATFORM KİLİTLEMESİ ---
        for (const auto& platform : platforms) {
            sf::FloatRect platBounds = platform.getGlobalBounds();
            if (playerBounds.position.x - (playerBounds.size.x / 2.0f) < platBounds.position.x + platBounds.size.x && 
                playerBounds.position.x + (playerBounds.size.x / 2.0f) > platform.getPosition().x && 
                playerBounds.position.y <= platBounds.position.y + 10.0f && 
                playerBounds.position.y >= platBounds.position.y - 20.0f) {
                
                if (platformJumpVelocity >= 0.0f) {
                    player.resetPosition(sf::Vector2f(playerBounds.position.x, platBounds.position.y));
                    playerBounds = player.getBounds(); 
                    if (jumpKeyPressed) {
                        platformJumpVelocity = -14.5f; 
                        if (hasJump) jumpSound.play(); 
                    }
                    else platformJumpVelocity = 0.0f;
                }
            }
        }

        // --- MERMİ HAREKETLERİ ---
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

        // --- DÜŞMAN TEMASI (HASAR ALMA) ---
        for (const auto& enemy : enemies) {
            if (enemy && enemy->sprite) {
                sf::FloatRect eBounds = enemy->sprite->getGlobalBounds();
                if (playerBounds.position.x - (playerBounds.size.x / 2.0f) < eBounds.position.x + eBounds.size.x && 
                    playerBounds.position.x + (playerBounds.size.x / 2.0f) > eBounds.position.x && 
                    playerBounds.position.y - playerBounds.size.y < eBounds.position.y + eBounds.size.y && 
                    playerBounds.position.y > eBounds.position.y) {
                    
                    if (hasHit) hitSound.play(); 
                    
                    health--; score = 0; 
                    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate, goldTexture, enemyTexture, blockTexture); 
                    player.resetPosition(sf::Vector2f(100.0f, 350.0f)); 
                    bullets.clear();
                    if (health <= 0) isGameOver = true; 
                    continue;
                }
            }
        }

        // --- ALTIN TOPLAMA ---
        for (size_t i = 0; i < coins.size(); ) {
            sf::FloatRect cBounds = coins[i].bounds;
            if (!coins[i].isCollected && 
                playerBounds.position.x - (playerBounds.size.x / 2.0f) < cBounds.position.x + cBounds.size.x && 
                playerBounds.position.x + (playerBounds.size.x / 2.0f) > cBounds.position.x && 
                playerBounds.position.y - playerBounds.size.y < cBounds.position.y + cBounds.size.y && 
                playerBounds.position.y > cBounds.position.y) { 
                
                if (hasCoin) coinSound.play(); 
                
                delete coins[i].sprite; coins.erase(coins.begin() + i); score += 10; 
            } else i++;
        }

        // --- RENDER (EKRANA ÇİZİM) ALANI ---
        window.clear(sf::Color::Black); 
        
        // 1. Oyun Alanı Çizimleri (Kamera Açısı Aktif)
        window.setView(camera);
        if (hasBg && bgWidth > 0.0f) { window.draw(bgSprite1); window.draw(bgSprite2); }   
        
        for (const auto& tile : groundTiles) {
            window.draw(tile);
        }

        window.draw(levelGate); 
        for (const auto& enemy : enemies) if (enemy && enemy->sprite) window.draw(*(enemy->sprite)); 
        for (const auto& b : bullets) window.draw(b);
        
        for (const auto& platform : platforms) {
            window.draw(platform);
        }
        
        for (const auto& c : coins) if (!c.isCollected) window.draw(*(c.sprite));
        player.draw(window); 

        if (showWarning) {
            warningText.setPosition(sf::Vector2f(camera.getCenter().x - 240.0f, 260.0f)); window.draw(warningText);
        }

        // 2. Arayüz (UI) Çizimleri (Sabit Görünüm Aktif - Kaymayı Önler)
        window.setView(uiView);
        uiText.setString("LEVEL: " + std::to_string(currentLevel) + "\n" + "SCORE: " + std::to_string(score));
        uiText.setPosition(sf::Vector2f(20.0f, 20.0f));
        window.draw(uiText); 

        if (hasHeartImg) {
            for (int i = 0; i < health; i++) {
                heartSprite.setPosition(sf::Vector2f(20.0f + (i * 38.0f), 94.0f)); 
                window.draw(heartSprite);
            }
        }
        window.display();           
    }  

    // --- BELLEK TEMİZLİĞİ ---
    for (auto& c : coins) delete c.sprite;
    for (auto& e : enemies) { if (e && e->sprite) delete e->sprite; delete e; }
    return 0;   
}