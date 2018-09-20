struct Inventory {
  enum Item { Nothing, Seeds, Hoe };

  sf::Texture texture;
  sf::Sprite sprite;
  sf::Texture selectedTexture;
  sf::Sprite selectedSprite;
  Item selectedItem;

  void loadTexture() {
    texture.loadFromFile("./assets/inventory.png");
    sprite.setTexture(texture);
    sprite.setScale(SCALE);
    selectedTexture.loadFromFile("./assets/selectedtile.png");
    selectedSprite.setTexture(selectedTexture);
    selectedSprite.setScale(SCALE);
    // display inventory centered, at bottom of screen
    sprite.setPosition(sf::Vector2f((WORLD_WIDTH / 2) - (30 * 5), WORLD_HEIGHT - (20 * 5)));
    setSelectedTexture();
  }

  void setSelectedTexture() {
    int pX;
    switch(selectedItem) {
      case(Nothing):
        pX = 30;
        break;
      case(Seeds):
        pX = 10;
        break;
      case(Hoe):
        pX = -10;
        break;
      default:
        break;
    }
    selectedSprite.setPosition(sf::Vector2f((WORLD_WIDTH / 2) - (pX * 5), WORLD_HEIGHT - (20 * 5)));
  }

  void handleKeys() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      selectedItem = Nothing;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      selectedItem = Seeds;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
      selectedItem = Hoe;
    }
    setSelectedTexture();
  }
};

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
    texture.loadFromFile("./assets/hero.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,20,40));
    sprite.setScale(SCALE);
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
    const bool isInBounds = (ngridX >= 0 && ngridX < WORLD_WIDTH / TILE_WIDTH && ngridY >= 0 && ngridY < WORLD_HEIGHT / TILE_WIDTH);

    // set coords of target tile and begin moving
    if (isInBounds) {
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

