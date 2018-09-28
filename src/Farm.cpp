struct Radish {
  int x;
  int y;
  int status;

  void water() {
    if (status < 2) {
      status++;
    }
  }
};

struct Plot {
  int x;
  int y;
};

struct Farm {
  sf::Texture cropsTexture;

  sf::Sprite radishSprite1;
  sf::Sprite radishSprite2;
  sf::Sprite radishSprite3;

  sf::Texture plotTexture;
  sf::Sprite plotSprite;
  vector<Radish> radishes;
  vector<Plot> plots;

  void loadTexture() {
    cropsTexture.loadFromFile("./assets/Crop_Spritesheet.png");
    radishSprite1.setTexture(cropsTexture);
    radishSprite2.setTexture(cropsTexture);
    radishSprite3.setTexture(cropsTexture);

    radishSprite1.setTextureRect(sf::IntRect(32,0,16,16));
    radishSprite2.setTextureRect(sf::IntRect(16,0,16,16));
    radishSprite3.setTextureRect(sf::IntRect(0,0,16,16));

    radishSprite1.setScale(SCALE);
    radishSprite2.setScale(SCALE);
    radishSprite3.setScale(SCALE);

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
      sf::Sprite radishSprite;
      if (r.status == 0) {
        radishSprite = radishSprite1;
      }
      if (r.status == 1) {
        radishSprite = radishSprite2;
      }
      if (r.status == 2) {
        radishSprite = radishSprite3;
      }
      radishSprite.setPosition(sf::Vector2f(r.x * TILE_WIDTH + 10, r.y * TILE_WIDTH));
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
      case(Inventory::WateringCan):
        waterTile(x, y);
        gameAudio.playWaterSound();
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

  void waterTile(int x, int y) {
    for(int i = 0; i < radishes.size(); i++) {
      Radish r = radishes[i];
      if (r.x == x && r.y == y) {
        r.water();
      }
      radishes[i] = r;
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
    r.status = 0;
    radishes.push_back(r);
    gameAudio.playSeedSound();
  }
};

