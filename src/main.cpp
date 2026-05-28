#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <string> 
#include <vector> 
#include <cmath>

// --- OYUNCUNUN FIRLATTIĞI MERMİLERİN YAPISI (STRUCT) ---
struct Bullet {
    sf::CircleShape shape; // Merminin ekrandaki yuvarlak şekli
    float speed;           // Merminin uçuş hızı
};

// --- GELİŞMİŞ CANAVAR YAPISI (STRUCT) ---
struct CustomEnemy {
    sf::RectangleShape shape; // Canavarın ekrandaki kutu şekli
    int type;                 // Canavar hareket tipi -> 0: Sağa-Sola Devriye, 1: Aşağı-Yukarı Süzülen
    float speedX;             // Yatay eksendeki hız bileşeni
    float speedY;             // Dikey eksendeki hız bileşeni
    float startX;             // Devriye atarken baz alacağı ilk X konumu
    float startY;             // Devriye atarken baz alacağı ilk Y konumu
    float patrolRange;        // Sağa sola ne kadar piksellik bir alanda gidip geleceği
};

// --- %100 SABİT VE BOL DÜŞMANLI EL YAPIMI BÖLÜM TASARIM MOTORU ---
// Bu fonksiyon çağrıldığında o anki level'a göre haritayı, paraları ve canavarları sıfırdan ve sabit yerlerine dizer.
void generateFixedLevel(int level, std::vector<sf::RectangleShape>& platforms, std::vector<CustomEnemy>& enemies, std::vector<sf::CircleShape>& coins, sf::RectangleShape& levelGate) {
    // Yeni bölüme geçerken eski harita verilerini tamamen temizliyoruz
    platforms.clear();
    enemies.clear();
    coins.clear();

    if (level == 1) {

        // Sabit platform koordinatları (X, Y)
        std::vector<sf::Vector2f> platPositions = {
            {250.0f, 450.0f}, {450.0f, 420.0f}, {700.0f, 460.0f}, 
            {950.0f, 430.0f}, {1200.0f, 400.0f}, {1450.0f, 450.0f}
        };
        // Platformları kalınlığı 10px ve eni 140px olacak şekilde vektöre dolduruyoruz
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(140.0f, 10.0f)); 
            plat.setFillColor(sf::Color(120, 120, 120)); // Gri renk
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Sağa Sola Giden Mor Canavarlar (Yatay Devriye)
        std::vector<sf::Vector2f> horizontalEnemies = {{400.0f, 280.0f}, {900.0f, 300.0f}};
        for (const auto& pos : horizontalEnemies) {
            CustomEnemy e; e.type = 0; e.shape.setSize(sf::Vector2f(35.0f, 25.0f));
            e.shape.setFillColor(sf::Color::Magenta); e.startX = pos.x; e.startY = pos.y;
            e.shape.setPosition(pos); e.speedX = 2.0f; e.speedY = 0.0f; e.patrolRange = 80.0f;
            enemies.push_back(e);
        }
        // Aşağı Yukarı Giden Sarı Canavarlar (Dikey Süzülme)
        std::vector<sf::Vector2f> verticalEnemies = {{600.0f, 320.0f}, {1150.0f, 280.0f}};
        for (const auto& pos : verticalEnemies) {
            CustomEnemy e; e.type = 1; e.shape.setSize(sf::Vector2f(35.0f, 25.0f));
            e.shape.setFillColor(sf::Color::Yellow); e.startX = pos.x; e.startY = pos.y;
            e.shape.setPosition(pos); e.speedX = 0.0f; e.speedY = 1.8f; e.patrolRange = 0.0f;
            enemies.push_back(e);
        }
    } 
    else if (level == 2) {
 
        // Bloklar biraz daha daralıyor (Eni: 95px) ve yükseklik farkları artıyor
        std::vector<sf::Vector2f> platPositions = {
            {250.0f, 480.0f}, {420.0f, 380.0f}, {600.0f, 460.0f}, 
            {800.0f, 350.0f}, {1000.0f, 440.0f}, {1200.0f, 360.0f}, {1400.0f, 450.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(95.0f, 10.0f)); 
            plat.setFillColor(sf::Color(100, 130, 150)); // Mavi-gri arası bir renk
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Sağa Sola Daha Hızlı Giden Turkuaz (Cyan) Canavarlar
        std::vector<sf::Vector2f> horizontalEnemies = {{350.0f, 260.0f}, {750.0f, 240.0f}, {1150.0f, 250.0f}};
        for (const auto& pos : horizontalEnemies) {
            CustomEnemy e; e.type = 0; e.shape.setSize(sf::Vector2f(35.0f, 25.0f));
            e.shape.setFillColor(sf::Color::Cyan); e.startX = pos.x; e.startY = pos.y;
            e.shape.setPosition(pos); e.speedX = 2.8f; e.speedY = 0.0f; e.patrolRange = 70.0f;
            enemies.push_back(e);
        }
        // Aşağı Yukarı Daha Seri Süzülen Sarı Canavarlar
        std::vector<sf::Vector2f> verticalEnemies = {{520.0f, 300.0f}, {950.0f, 280.0f}};
        for (const auto& pos : verticalEnemies) {
            CustomEnemy e; e.type = 1; e.shape.setSize(sf::Vector2f(35.0f, 25.0f));
            e.shape.setFillColor(sf::Color::Yellow); e.startX = pos.x; e.startY = pos.y;
            e.shape.setPosition(pos); e.speedX = 0.0f; e.speedY = 2.4f; e.patrolRange = 0.0f;
            enemies.push_back(e);
        }
    } 
    else if (level == 3) {
   
        // Bloklar nokta kadar küçülüyor (Eni: 65px) ve tam bir beceri testi sunuyor
        std::vector<sf::Vector2f> platPositions = {
            {250.0f, 460.0f}, {400.0f, 400.0f}, {550.0f, 340.0f}, {700.0f, 440.0f}, 
            {850.0f, 380.0f}, {1000.0f, 460.0f}, {1150.0f, 350.0f}, {1300.0f, 420.0f}, {1450.0f, 470.0f}
        };
        for (const auto& pos : platPositions) {
            sf::RectangleShape plat(sf::Vector2f(65.0f, 10.0f)); 
            plat.setFillColor(sf::Color(180, 80, 80)); // Kırmızımsı tehlike rengi
            plat.setPosition(pos);
            platforms.push_back(plat);
        }

        // Sağa Sola Deliler Gibi Çıldıran Kırmızı Canavarlar
        std::vector<sf::Vector2f> horizontalEnemies = {{350.0f, 220.0f}, {750.0f, 260.0f}, {1150.0f, 210.0f}};
        for (const auto& pos : horizontalEnemies) {
            CustomEnemy e; e.type = 0; e.shape.setSize(sf::Vector2f(35.0f, 25.0f));
            e.shape.setFillColor(sf::Color::Red); e.startX = pos.x; e.startY = pos.y;
            e.shape.setPosition(pos); e.speedX = 3.8f; e.speedY = 0.0f; e.patrolRange = 90.0f;
            enemies.push_back(e);
        }
        // Yıldırım Hızıyla Aşağı Yukarı İnen Sarı Canavarlar
        std::vector<sf::Vector2f> verticalEnemies = {{480.0f, 250.0f}, {920.0f, 240.0f}, {1350.0f, 260.0f}};
        for (const auto& pos : verticalEnemies) {
            CustomEnemy e; e.type = 1; e.shape.setSize(sf::Vector2f(35.0f, 25.0f));
            e.shape.setFillColor(sf::Color::Yellow); e.startX = pos.x; e.startY = pos.y;
            e.shape.setPosition(pos); e.speedX = 0.0f; e.speedY = 3.3f; e.patrolRange = 0.0f;
            enemies.push_back(e);
        }
    }

    // --- SAFKAN HAVA PARKURU: ALTINLARI BLOKLARIN SIRTINA YERLEŞTİRME ---
    // Yerde hiç altın yok, her bloğun tam matematiksel ortasına bir sarı para konduruyoruz
    for (const auto& plat : platforms) {
        sf::CircleShape c(10.0f); 
        c.setFillColor(sf::Color::Yellow);
        // Formül: Bloğun başlangıç X konumu + (Bloğun Genişliğinin Yarısı) - Paranın Yarıçapı = Tam Ortalanmış Para
        c.setPosition(sf::Vector2f(plat.getPosition().x + (plat.getSize().x / 2.0f) - 10.0f, plat.getPosition().y - 32.0f));
        coins.push_back(c);
    }

    // Yeşil bitiş kapısını her zaman haritadaki en son bloğun 220 piksel ilerisine sabitliyoruz
    levelGate.setPosition(sf::Vector2f(platforms.back().getPosition().x + 220.0f, 400.0f));
}

int main() {
    // 800x600 piksel boyutunda oyun penceresi oluşturuluyor
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Action Platformer - Monster Mayhem");
    window.setFramerateLimit(60); // Oyunun hızı her bilgisayarda eşit olsun diye 60 FPS sınırı koyuyoruz

    // Oyuncuyu takip edecek dinamik kamera (View) sistemi
    sf::View camera(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800.0f, 600.0f)));

    // Font yükleme işlemi (Mac sistem yazı tipleri destekli)
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        font.openFromFile("/System/Library/Fonts/Courier.dfont");
    }

    // Sol üst köşedeki Skor, Can ve Bölüm yazı göstergesi
    sf::Text uiText(font);
    uiText.setCharacterSize(26);            
    uiText.setFillColor(sf::Color::White);  

    // Kapı kilitliyken ekrana çıkacak olan kırmızı uyarı yazısı
    sf::Text warningText(font);
    warningText.setCharacterSize(22);
    warningText.setFillColor(sf::Color::Red);

    // Oyun bitti veya kazandın durumunda ekranda çıkacak devasa orta metin
    sf::Text centerText(font);
    centerText.setCharacterSize(42);
    centerText.setFillColor(sf::Color::Red);

    // Ana Katı Zemin (Kamera sağa kayacağı için uzunluğunu 5000px yapıyoruz)
    sf::RectangleShape ground(sf::Vector2f(5000.0f, 50.0f));
    ground.setFillColor(sf::Color(50, 50, 50));
    ground.setPosition(sf::Vector2f(0.0f, 550.0f));

    // Yeşil dikey Bölüm Geçiş Kapısı direği
    sf::RectangleShape levelGate(sf::Vector2f(20.0f, 150.0f));
    levelGate.setFillColor(sf::Color::Green);

    // Dinamik oyun listeleri (Vektörler)
    std::vector<sf::RectangleShape> platforms;
    std::vector<CustomEnemy> enemies;
    std::vector<sf::CircleShape> coins;

    // Oyunu Bölüm 1'den başlatıyoruz ve haritayı yüklüyoruz
    int currentLevel = 1;
    generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate);

    // Karakterimizi (Boyut: 50x50, Başlangıç Konumu: X=100, Y=400) oluşturuyoruz
    Player player(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(100.0f, 400.0f));

    std::vector<Bullet> bullets; // Ekrandaki aktif mermilerin listesi
    sf::Clock shootClock;        // Seri ateş etmeyi engellemek için cooldown süreölçeri
    float shootCooldown = 0.25f; // İki mermi arası beklenecek saniye (Ateş hızı sınırı)

    // Temel Oyun Değişkenleri
    int score = 0;
    int health = 5;       
    int levelStartScore = 0;    // Öldüğünde o bölümdeki puanı sıfırlayabilmek için bölüm başı skoru tutuyoruz
    bool isGameOver = false;    // Kaybetme durumu kontrolü
    bool isGameWon = false;     // Tamamen kazanma durumu kontrolü
    bool showWarning = false;   // Kapı uyarısı ekranda görünsün mü bayrağı
    float platformJumpVelocity = 0.0f; // Blok üstünde art arda zıplayabilmek için dikey ivme motoru

    // --- ANA OYUN DÖNGÜSÜ ---
    while (window.isOpen()) {
        // Pencere kapatılma olaylarını (X butonuna basılması) yakalıyoruz
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // --- GAME OVER (KAYBETME) EKRANI MANTIĞI ---
        if (isGameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                // R tuşuna basılırsa tüm değişkenleri sıfırla ve Bölüm 1'den baştan başlat
                score = 0; health = 5; currentLevel = 1; levelStartScore = 0; isGameOver = false; isGameWon = false; showWarning = false;
                platformJumpVelocity = 0.0f;
                camera.setCenter(sf::Vector2f(400.0f, 300.0f));
                bullets.clear();
                generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate);
                player.resetPosition(sf::Vector2f(100.0f, 400.0f));
            }
            window.setView(window.getDefaultView()); // Kamerayı ekrana sabitle ki yazı kaymasın
            window.clear(sf::Color::Black);
            centerText.setString("GAME OVER\nYeniden baslamak icin R'ye bas");
            centerText.setPosition(sf::Vector2f(100.0f, 240.0f));
            window.draw(centerText);
            window.display();
            continue; // Döngünün geri kalanını çalıştırma, bu ekranda kal
        }

        // --- VICTORY (TAMAMEN KAZANMA) EKRANI MANTIĞI ---
        if (isGameWon) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                score = 0; health = 5; currentLevel = 1; levelStartScore = 0; isGameOver = false; isGameWon = false; showWarning = false;
                platformJumpVelocity = 0.0f;
                camera.setCenter(sf::Vector2f(400.0f, 300.0f));
                bullets.clear();
                generateFixedLevel(currentLevel, platforms, enemies, coins, levelGate);
                player.resetPosition(sf::Vector2f(100.0f, 400.0f));
            }
            window.setView(window.getDefaultView());
            window.clear(sf::Color(15, 60, 15)); // Koyu yeşil zafer ekranı
            centerText.setString("TEBRIKLER! OYUNU TAMAMEN BITIRDIN!\nYeniden oynamak icin R'ye bas");
            centerText.setPosition(sf::Vector2f(60.0f, 240.0f));
            window.draw(centerText);
            window.display();
            continue;
        }

        // Klavye zıplama tuşlarını kontrol ediyoruz (Space veya Yukarı Ok Tuşu)
        bool jumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);

        // --- KLAVYEDEN KARAKTER HAREKET KONTROLLERİ (A - D / Yön Tuşları) ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x - 5.0f, player.getBounds().position.y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x + 5.0f, player.getBounds().position.y));
        }

        // --- F TUŞU İLE SİLAH/MERMİ ATEŞLEME MEKANİZMASI ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
            Bullet newBullet;
            newBullet.shape.setRadius(5.0f); // Küçük mermi dairesi
            newBullet.shape.setFillColor(sf::Color::Cyan); // Turkuaz parlak renk
            sf::FloatRect pB = player.getBounds();
            // Merminin çıkış noktasını karakterin tam önü ve dikey ortası yapıyoruz
            newBullet.shape.setPosition(sf::Vector2f(pB.position.x + pB.size.x, pB.position.y + (pB.size.y / 2.0f) - 5.0f));
            newBullet.speed = 11.0f; // Sağa doğru fişekleme hızı
            bullets.push_back(newBullet);
            shootClock.restart(); // Cooldown süresini baştan başlat
        }

        player.update(); // Sınıf içi fizik ve yerçekimi güncelleniyor

        // --- BLOK ÜSTÜ ZIPLAMA FİZİK SİMÜLASYONU ---
        if (platformJumpVelocity < 0.0f) {
            player.resetPosition(sf::Vector2f(player.getBounds().position.x, player.getBounds().position.y + platformJumpVelocity));
            platformJumpVelocity += 0.6f; // Yukarı fırlayan karakteri yerçekimiyle yavaşlatıyoruz
        }

        // --- DİNAMİK KAMERA KAYDIRMA (CAMERA SCROLLING) ---
        // Karakter ekranın ortasını (400px) geçince kamera karakteri merkeze alıp sağa doğru kayar.
        float playerX = player.getBounds().position.x;
        if (playerX > 400.0f) {
            camera.setCenter(sf::Vector2f(playerX, 300.0f));
        } else {
            camera.setCenter(sf::Vector2f(400.0f, 300.0f));
        }
        window.setView(camera); // Kamerayı ekrana giydir

        sf::FloatRect playerBounds = player.getBounds();

        // --- AKILLI CANAVAR HAREKET MOTORU ---
        for (size_t i = 0; i < enemies.size(); i++) {
            if (enemies[i].type == 0) { 
                // Tip 0: Yatayda kendi sınırları (patrolRange) içinde sağa sola devriye atar
                enemies[i].shape.move(sf::Vector2f(enemies[i].speedX, 0.0f));
                if (enemies[i].shape.getPosition().x < enemies[i].startX - enemies[i].patrolRange) enemies[i].speedX = std::abs(enemies[i].speedX);
                else if (enemies[i].shape.getPosition().x > enemies[i].startX + enemies[i].patrolRange) enemies[i].speedX = -std::abs(enemies[i].speedX);
            } 
            else { 
                // Tip 1: Dikeyde havada yumuşak bir şekilde 60 piksel aşağı-yukarı süzülür
                enemies[i].shape.move(sf::Vector2f(0.0f, enemies[i].speedY));
                if (enemies[i].shape.getPosition().y < enemies[i].startY - 60.0f) enemies[i].speedY = std::abs(enemies[i].speedY);
                else if (enemies[i].shape.getPosition().y > enemies[i].startY + 60.0f) enemies[i].speedY = -std::abs(enemies[i].speedY);
            } 
        }
// 60 PUAN BARAJI İLE BÖLÜM GEÇİŞİ
        showWarning = false; 
        if (playerBounds.findIntersection(levelGate.getGlobalBounds()).has_value()) {
            int scoreCollectedInThisLevel = score - levelStartScore;

            if (scoreCollectedInThisLevel >= 60) { 
                if (currentLevel < 3) {
                    currentLevel++; 
                    levelStartScore = score; 
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

  
        for (const auto& platform : platforms) {
            sf::FloatRect platBounds = platform.getGlobalBounds();
            if (playerBounds.findIntersection(platBounds).has_value()) {
                // Oyuncu düşerken ayağı bloğun üst yüzey sınırına denk gelirse
                if (playerBounds.position.y + playerBounds.size.y <= platBounds.position.y + 14.0f) {
                    // Karakteri bloğun tam sırtına oturt ve düşüşü kes
                    player.resetPosition(sf::Vector2f(playerBounds.position.x, platBounds.position.y - playerBounds.size.y));
                    playerBounds = player.getBounds(); 

                    // Blok üstündeyken zıplanırsa ivme motorunu ateşle!
                    if (jumpKeyPressed) {
                        platformJumpVelocity = -14.5f; 
                    } else if (platformJumpVelocity > 0.0f) {
                        platformJumpVelocity = 0.0f; 
                    }
                }
            }
        }

        // --- SİLAH MERMİLERİNİN İLERLEMESİ VE CANAVARLARI VURMA KONTROLÜ ---
        for (size_t bIdx = 0; bIdx < bullets.size(); bIdx++) {
            bullets[bIdx].shape.move(sf::Vector2f(bullets[bIdx].speed, 0.0f)); // Mermiyi sağa kaydır
            sf::FloatRect bulletBounds = bullets[bIdx].shape.getGlobalBounds();
            bool bulletDestroyed = false;

            for (size_t eIdx = 0; eIdx < enemies.size(); eIdx++) {
                // Mermi bir canavara çarptıysa
                if (bulletBounds.findIntersection(enemies[eIdx].shape.getGlobalBounds()).has_value()) {
                    score += 20; // Canavarı öldürmek +20 puan ödül verir
                    enemies.erase(enemies.begin() + eIdx); // Canavarı dünyadan sil
                    bullets.erase(bullets.begin() + bIdx); // Mermiyi patlatıp yok et
                    bulletDestroyed = true;
                    break;
                }
            }
            if (bulletDestroyed) break;

            // Ekrandan çok uzağa uçan mermileri bellek birikmesin diye listeden temizliyoruz
            if (bullets[bIdx].shape.getPosition().x > camera.getCenter().x + 500.0f) {
                bullets.erase(bullets.begin() + bIdx);
                break;
            }
        }


        for (auto& enemy : enemies) {
            if (playerBounds.findIntersection(enemy.shape.getGlobalBounds()).has_value()) {
                health--; // Bir can kaybet
                
     
                score = levelStartScore; 
                
                // Karakteri kameranın sol tarafına güvenli bir konuma geri fırlatır
                player.resetPosition(sf::Vector2f(camera.getCenter().x - 300.0f, 400.0f)); 
                platformJumpVelocity = 0.0f;
                if (health <= 0) isGameOver = true; // Can bittiyse Game Over bayrağını kaldır
            }
        }

        // --- 🪙 ALTIN TOPLAMA SİSTEMİ ---
        for (size_t i = 0; i < coins.size(); i++) {
            if (playerBounds.findIntersection(coins[i].getGlobalBounds()).has_value()) {
                score += 10; // Her altın +10 puan ekler
                coins.erase(coins.begin() + i); // Toplanan altını ekrandan kaldır
                break; 
            }
        }

        // Sol üstteki canlı HUD/Arayüz metnini güncelliyoruz
        uiText.setString("Puan: " + std::to_string(score) + "   Can: " + std::to_string(health) + "   Bolum: " + std::to_string(currentLevel));
        // Yazı göstergelerinin kamera kaydıkça sol üstte sabit kalması için pozisyonunu güncelliyoruz
        uiText.setPosition(sf::Vector2f(camera.getCenter().x - 380.0f, 20.0f));

        // Kapı kilitliyse kırmızı renkli ara uyarı yazısını konumlandır ve göster
        if (showWarning) {
            int currentLevelScore = score - levelStartScore;
            warningText.setString("KAPI KILITLI! Bu bolumde " + std::to_string(currentLevelScore) + "/60 altin topladin!");
            warningText.setPosition(sf::Vector2f(camera.getCenter().x - 200.0f, 60.0f));
        }

        // --- RENDER (EKRANA ÇİZİM) AŞAMASI ---
        window.clear(sf::Color::Black); // Ekranı her kare başında siyah renkle temizle
        
        window.draw(ground);    // Zemini çiz        
        window.draw(levelGate); // Bölüm sonu yeşil kapıyı çiz
        
        // Tüm dinamik nesneleri döngülerle ekrana basıyoruz
        for (const auto& enemy : enemies) window.draw(enemy.shape);
        for (const auto& b : bullets) window.draw(b.shape);
        for (const auto& platform : platforms) window.draw(platform);
        for (const auto& c : coins) window.draw(c);

        player.draw(window);     // Karakteri çiz       
        window.draw(uiText);     // Göstergeleri çiz
        
        if (showWarning) window.draw(warningText); // Uyarı aktifse ekrana bas
        
        window.display(); // Arka planda çizilen her şeyi canlı ekrana yansıt
    }

    return 0;
} 