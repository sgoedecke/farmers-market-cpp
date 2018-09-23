struct Radish {
  int x;
  int y;
};

struct Plot {
  int x;
  int y;
};

struct Farm {
  sf::Texture radishTexture;
  sf::Sprite radishSprite;
  sf::Texture plotTexture;
  sf::Sprite plotSprite;
  vector<Radish> radishes;
  vector<Plot> plots;

  void loadTexture() {
    radishTexture.loadFromFile("./assets/crop.png");
    radishSprite.setTexture(radishTexture);
    radishSprite.setScale(SCALE);
    plotTexture.loadFromFile("./assets/plot.png");
    plotSprite.setTexture(plotTexture);
    plotSprite.setScale(SCALE);
  }

  void drawInto(sf::RenderWindow* w) {
    for(Plot p : plots) {
      plotSprite.setPosition(sf::Vector2f(p.x * TILE_WIDTH, p.y * TILE_WIDTH));
      w->draw(plotSprite);
    }

    for(Radish r : radishes) {
      radishSprite.setPosition(sf::Vector2f(r.x * TILE_WIDTH, r.y * TILE_WIDTH));
      w->draw(radishSprite);
    }
  }

  bool plotAtTile(int x, int y) {
    for(Plot p : plots) {
      if (p.x == x && p.y == y) {
        return true;
      }
    }
    return false;
  }

  void interactWithTile(int x, int y, Inventory::Item item) {
    // validate target tile 
    const bool isInBounds = (x > 0 && x < (WORLD_WIDTH / TILE_WIDTH) - 1 && y > 0 && y < (WORLD_HEIGHT / TILE_WIDTH) - 1);
    if (!isInBounds) {
      return;
    }

    switch(item) {
      case(Inventory::Nothing):
        break;
      case(Inventory::Seeds):
        if (plotAtTile(x, y)) {
          plantRadish(x, y);
        } else {
          gameAudio.playErrorSound();
        }
        break;
      case(Inventory::Hoe):
        if (!plotAtTile(x, y)) {
          digPlot(x, y);
        } 
        break;
      default:
        break;
    }
  }

  void digPlot(int x, int y) {
    Plot p;
    p.x = x;
    p.y = y;
    plots.push_back(p);
    gameAudio.playHoeSound();
  }

  void plantRadish(int x, int y) {
    Radish r;
    r.x = x;
    r.y = y;
    radishes.push_back(r);
    gameAudio.playSeedSound();
  }
};

