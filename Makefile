# Derleyici ve Standart
CXX = g++
CXXFLAGS = -std=c++17

# Hedef Çalıştırılabilir Dosya Adı
TARGET = game

# Kaynak Dosyaları
SRCS = src/main.cpp src/Player.cpp

# Derleme Kuralı
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Temizlik Kuralı
clean:
	rm -f $(TARGET)