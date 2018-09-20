struct SelectedTile {
  int x, y; // the grid position, not pixel positon
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
    switch(dir) {
      case Dir::Up:
        y -= 1;
        break;
      case Dir::Down:
        y += 1;
        break;
      case Dir::Left:
        x -= 1;
        break;
      case Dir::Right:
        x += 1;
        break;
    }
    // snap to grid
    totalLife = 10;
    tickCount = 0;
    isActive = true; 
    sprite.setPosition(sf::Vector2f(x * TILE_WIDTH, y * TILE_WIDTH));
  }

  void tick() {
    tickCount++;
    if (tickCount >= totalLife) {
      isActive = false;
    }
  }
};

