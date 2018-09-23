#include <SFML/Audio.hpp>

struct Audio {
  sf::Music music;
  sf::SoundBuffer hoeBuffer;
  sf::SoundBuffer seedBuffer;
  sf::SoundBuffer errorBuffer;
  sf::Sound sound;

  void loadBuffers() {
    music.openFromFile("./assets/audio/wandering-the-path-punch-deck.wav");
    hoeBuffer.loadFromFile("./assets/audio/hoe.wav");
    seedBuffer.loadFromFile("./assets/audio/seeds.wav");
    errorBuffer.loadFromFile("./assets/audio/error.wav");
  }

  void playHoeSound() {
    sound.setBuffer(hoeBuffer);
    sound.play();
  }

  void playSeedSound() {
    sound.setBuffer(seedBuffer);
    sound.play();
  }

  void playErrorSound() {
    sound.setBuffer(errorBuffer);
    sound.play();
  }

  void playMusic() {
    music.play();
    music.setVolume(20);
    music.setLoop(true);
  }
};
