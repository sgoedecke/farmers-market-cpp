struct Market {
  sf::Texture texture;
  sf::Sprite sprite;
  int animTick;
  int customers;
  int maxCustomers;
  vector<Crop> goods;

  void loadTexture() {
    animTick = 0;
    customers = 0;
    maxCustomers = 5;
    texture.loadFromFile("./assets/market-bg.png");
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(15.f, 15.f));
  }

  void tick() {
    animTick++;
  }

  void drawInto(sf::RenderWindow* w) {
    w->draw(sprite);
  }
};
