struct Radish {
  int x;
  int y;
};

struct CropManager {
  sf::Texture radishTexture;
  sf::Sprite radishSprite;
  vector<Radish> radishes;

  void loadTexture() {
    radishTexture.loadFromFile("./assets/crop.png");
    radishSprite.setTexture(radishTexture);
    radishSprite.setScale(SCALE);
  }

  void drawInto(sf::RenderWindow* w) {
    for(Radish r : radishes) {
        radishSprite.setPosition(sf::Vector2f(r.x * TILE_WIDTH, r.y * TILE_WIDTH));
        w->draw(radishSprite);
    }
  }

  void plantRadish(int x, int y) {
    Radish r;
    r.x = x;
    r.y = y;
    radishes.push_back(r);
  }
};

