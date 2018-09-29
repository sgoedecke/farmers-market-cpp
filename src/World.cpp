struct World {
  sf::Texture texture;
  sf::Sprite sprite;

  void loadTexture() {
    texture.loadFromFile("./assets/world-bg-full.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,WORLD_WIDTH,WORLD_HEIGHT));
    sprite.setScale(sf::Vector2f(5.f, 5.f));
  }

};

