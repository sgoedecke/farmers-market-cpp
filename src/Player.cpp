struct Player {
  float x, y;
  float speed;
  bool isMoving;
  Dir::Type dir;
  sf::Texture texture;
  sf::Sprite sprite;

  void loadTexture() {
    texture.loadFromFile("./assets/hero.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,20,40));
    sprite.setScale(SCALE);
  }

  void beginMove(Dir::Type d) {
    dir = d;
    isMoving = true;
  }

  void setTexture() {
    int tX;
    switch(dir) {
      case Dir::Up:
        tX = 3;
        break;
      case Dir::Down:
        tX = 6;
        break;
      case Dir::Left:
        tX = 9;
        break;
      case Dir::Right:
        tX = 0;
        break;
    }
    tX = tX + textureTick;
    sprite.setTextureRect(sf::IntRect((tX * 19), 0, 18, 40));
  }

  void move() {
    int nx = x;
    int ny = y;
    if (!isMoving) {
      return;
    }
    switch(dir) {
      case Dir::Up:
        ny -= speed;
        break;
      case Dir::Down:
        ny += speed;
        break;
      case Dir::Left:
        nx -= speed;
        break;
      case Dir::Right:
        nx += speed;
        break;
    }
    const bool isInBounds = (nx >= 0 && nx + TILE_WIDTH < WORLD_WIDTH && ny >= 0 && ny + (TILE_WIDTH * 2) < WORLD_HEIGHT);
    if (isInBounds) {
      setTexture();
      x = nx;
      y = ny;
      sprite.setPosition(sf::Vector2f(x, y));
    }
  }
};

