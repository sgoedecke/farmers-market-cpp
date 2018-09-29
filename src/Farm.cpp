struct Radish {
  int x;
  int y;
  int status;
};

struct Plot {
  int x;
  int y;
  bool watered;
};

struct Rock {
  int x;
  int y;
};

struct HarvestedCrops {
  int radishes;
};

struct Farm {
  sf::Texture cropsTexture;

  sf::Sprite radishSprite1;
  sf::Sprite radishSprite2;
  sf::Sprite radishSprite3;

  sf::Texture plotTexture;
  sf::Texture plotWateredTexture;
  sf::Sprite plotSprite;
  sf::Sprite plotWateredSprite;

  sf::Sprite rockSprite;
  sf::Texture rockTexture;

  vector<Radish> radishes;
  vector<Plot> plots;
  vector<Rock> rocks;

  HarvestedCrops harvestedCrops;
  Inventory* inventoryPtr;

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

    rockTexture.loadFromFile("./assets/rock.png");
    rockSprite.setTexture(rockTexture);
    rockSprite.setScale(SCALE);

    plotTexture.loadFromFile("./assets/plot.png");
    plotSprite.setTexture(plotTexture);

    plotWateredTexture.loadFromFile("./assets/plot-watered.png");
    plotWateredSprite.setTexture(plotWateredTexture);
    plotSprite.setScale(SCALE);
    plotWateredSprite.setScale(SCALE);
  }

  void drawInto(sf::RenderWindow* w) {
    for(Plot p : plots) {
      if (p.watered) {
        plotWateredSprite.setPosition(sf::Vector2f(p.x * TILE_WIDTH, p.y * TILE_WIDTH));
        w->draw(plotWateredSprite);
      } else {
        plotSprite.setPosition(sf::Vector2f(p.x * TILE_WIDTH, p.y * TILE_WIDTH));
        w->draw(plotSprite);
     }
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

    for(Rock r: rocks) {
      rockSprite.setPosition(sf::Vector2f(r.x * TILE_WIDTH, r.y * TILE_WIDTH));
      w->draw(rockSprite);
    }
  }

  bool isPassable(int x, int y) {
    if (rockAtTile(x, y)) {
      return false;
    }
    for(Radish r : radishes) {
      if (r.x == x && r.y == y && r.status == 2) {
        return false;
      }
    }
    return true;
  }

  bool rockAtTile(int x, int y) {
    for(Rock r : rocks) {
      if (r.x == x && r.y == y) {
        return true;
      }
    }
    return false;
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
        harvestTile(x, y);
        break;
      case(Inventory::WateringCan):
        waterTile(x, y);
        gameAudio.playWaterSound();
        break;
      case(Inventory::Fertilizer):
        fertilizeTile(x, y);
        gameAudio.playWaterSound();
        break;
      case(Inventory::Seeds):
        if (plotAtTile(x, y)) {
          plantRadish(x, y);
        } else {
          gameAlert.setMessage("Use your hoe!");
          gameAudio.playErrorSound();
        }
        break;
      case(Inventory::Hoe):
        if (rockAtTile(x, y)) {
          removeRock(x, y);
        } else if (!plotAtTile(x, y)) {
          digPlot(x, y);
        } 
        break;
      default:
        break;
    }
  }

  void harvestTile(int x, int y) {
    for(int i = 0; i < radishes.size(); i++) {
      if (radishes[i].status == 2 && radishes[i].x == x && radishes[i].y == y) {
        gameAlert.setMessage("Got a radish!");
        harvestedCrops.radishes++;
        radishes.erase(radishes.begin() + i);
      }
    }
  }

  void removeRock(int x, int y) {
    for(int i = 0; i < rocks.size(); i++) {
      if (rocks[i].x == x && rocks[i].y == y) {
        rocks.erase(rocks.begin() + i);
      }
    }
    animations.spawnAnimation(x, y, Animation::Type::RemoveRock);
  }

  void fertilizeTile(int x, int y) {
	if(inventoryPtr->numFertilizer < 1) {
      gameAlert.setMessage("No fertilizer!");
	  return;
	}

    for(int i = 0; i < radishes.size(); i++) {
      if (radishes[i].x == x && radishes[i].y == y && radishes[i].status < 2) {
        radishes[i].status++;
		inventoryPtr->numFertilizer--;
      }
    }
  }

  void waterTile(int x, int y) {
    for(int i = 0; i < plots.size(); i++) {
      if(plots[i].x == x && plots[i].y == y) {
        plots[i].watered = true;
      }
    }
    animations.spawnAnimation(x, y, Animation::Type::Water);
  }

  void digPlot(int x, int y) {
    Plot p;
    p.x = x;
    p.y = y;
    p.watered = false;
    plots.push_back(p);
    gameAudio.playHoeSound();
    animations.spawnAnimation(x, y, Animation::Type::Dig);
  }

  void plantRadish(int x, int y) {
	if(inventoryPtr->numSeeds < 1) {
      gameAlert.setMessage("No seeds!");
	  return;
	}

    for(Radish r : radishes) {
      if (r.x == x && r.y == y) {
        return; // since there's already a radish here
      }
    }

    // create a new radish
    Radish r;
    r.x = x;
    r.y = y;
    r.status = 0;
    radishes.push_back(r);
    gameAudio.playSeedSound();
	inventoryPtr->numSeeds--;
  }

  void spawnRock(int x, int y) {
    Rock r;
    r.x = x;
    r.y = y;
    rocks.push_back(r);
  }
};

