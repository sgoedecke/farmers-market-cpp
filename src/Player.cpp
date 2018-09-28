struct Player {
  float x, y;
  int gridX, gridY;
  float speed;
  bool isMoving;
  Dir::Type dir;
  sf::Texture texture;
  sf::Sprite sprite;
  Inventory inventory;

  void loadTexture() {
    gridX = 10;
    gridY = 10;
    x = gridX * TILE_WIDTH;
    y = gridY * TILE_WIDTH;
    dir = Dir::Up;
    texture.loadFromFile("./assets/hero.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,20,40));
    sprite.setScale(SCALE);
    setTexture();
    setPosition();
    inventory.loadTexture();
  }

  void beginMove(Dir::Type d) {
    // if we're currently moving, wait for it to finish
    // might have to rethink this if it makes moving janky
    if (isMoving) {
      return;
    }

    // determine target tile
    int ngridX = gridX;
    int ngridY = gridY;
    switch(d) {
      case Dir::Up:
        ngridY--;
        break;
      case Dir::Down:
        ngridY++;
        break;
      case Dir::Left:
        ngridX--;
        break;
      case Dir::Right:
        ngridX++;
        break;
    }
    // validate target tile 
    const bool isInBounds = (ngridX > 0 && ngridX < (WORLD_WIDTH / TILE_WIDTH) - 1 && ngridY > 0 && ngridY < (WORLD_HEIGHT / TILE_WIDTH) - 1);
    const bool isPassable = farm.isPassable(ngridX, ngridY);

    // set coords of target tile and begin moving
    if (isInBounds && isPassable) {
      dir = d;
      isMoving = true;
      gridX = ngridX;
      gridY = ngridY;
    }
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

  bool isAtTarget() {
    return (abs(x - gridX * TILE_WIDTH) < speed && abs(y - gridY * TILE_WIDTH) < speed);
  }

  void handleKeys() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      beginMove(Dir::Up);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      beginMove(Dir::Left);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      beginMove(Dir::Down);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      beginMove(Dir::Right);
    }
    inventory.handleKeys();
  }

  void setPosition() {
    // since our sprite is slightly larger than a tile, draw the feet on the player
    // position and let the head spill over
    sprite.setPosition(sf::Vector2f(x, y - TILE_WIDTH / 2));
  }

  void move() {
    if (!isMoving) {
      return;
    }
    // if we're close enough to the grid target, snap to it
    if (isAtTarget()) {
      x = gridX * TILE_WIDTH;
      y = gridY * TILE_WIDTH; 
      isMoving = false;

      // if a key's pressed, start a new move right away without waiting for the next tick
      handleKeys();
    } else {
      // move towards the target
      switch(dir) {
        case Dir::Up:
          y -= speed;
          break;
        case Dir::Down:
          y += speed;
          break;
        case Dir::Left:
          x -= speed;
          break;
        case Dir::Right:
          x += speed;
          break;
      }
    }

    setTexture();
    setPosition();
  }

  void drawInto(sf::RenderWindow* w) {
    w->draw(sprite);
    w->draw(inventory.sprite);
    w->draw(inventory.selectedSprite);
  }
};

