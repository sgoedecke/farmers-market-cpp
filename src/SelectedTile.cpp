struct SelectedTile {
  int x, y;
  int totalLife;
  int tickCount;
  bool isActive;

  sf::Texture texture;
  sf::Sprite sprite;

  void loadTexture() {
    texture.loadFromFile("./assets/selectedtile.png");
    sprite.setTexture(texture);
    sprite.setScale(SCALE);
  }

  void reset(int nx, int ny, Dir::Type dir) {
    x = nx;
    y = ny;
    y += TILE_WIDTH; // adjust tile to be closer to feet than head
    switch(dir) {
      case Dir::Up:
        y -= TILE_WIDTH;
        break;
      case Dir::Down:
        y += TILE_WIDTH;
        break;
      case Dir::Left:
        x -= TILE_WIDTH;
        break;
      case Dir::Right:
        x += TILE_WIDTH;
        break;
    }
    // snap to grid
    x = (x / TILE_WIDTH) * TILE_WIDTH;
    y = (y / TILE_WIDTH) * TILE_WIDTH;
    totalLife = 10;
    tickCount = 0;
    isActive = true; 
    sprite.setPosition(sf::Vector2f(x, y));
  }

  void tick() {
    tickCount++;
    if (tickCount >= totalLife) {
      isActive = false;
    }
  }
};

