struct Crop {
  enum Type { Radish, Eggplant, Corn };
  int x;
  int y;
  int status;
  Type type;
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
  int eggplant;
  int corn;
};

struct Farm {
  sf::Texture cropsTexture;

  sf::Sprite cropSprite;

  sf::Texture plotTexture;
  sf::Texture plotWateredTexture;
  sf::Sprite plotSprite;
  sf::Sprite plotWateredSprite;

  sf::Sprite rockSprite;
  sf::Texture rockTexture;

  vector<Crop> crops;
  vector<Plot> plots;
  vector<Rock> rocks;

  HarvestedCrops harvestedCrops;
  Inventory* inventoryPtr;

  void loadTexture() {
    cropsTexture.loadFromFile("./assets/Crop_Spritesheet.png");

    cropSprite.setTexture(cropsTexture);
    cropSprite.setScale(SCALE);

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

    for(Crop r : crops) {
      int tY;
      switch (r.type) {
		case Crop::Type::Radish:
          tY = 0;
		  break;
		case Crop::Type::Eggplant:
		  tY = 3 * 16;
		  break;
		case Crop::Type::Corn:
          tY = 9 * 16;
		  break;
	  }
      if (r.status == 0) {
		cropSprite.setTextureRect(sf::IntRect(32,tY,16,16));
      }
      if (r.status == 1) {
		cropSprite.setTextureRect(sf::IntRect(16,tY,16,16));
      }
      if (r.status == 2) {
		cropSprite.setTextureRect(sf::IntRect(0,tY,16,16));
      }
      cropSprite.setPosition(sf::Vector2f(r.x * TILE_WIDTH + 10, r.y * TILE_WIDTH));
      w->draw(cropSprite);
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
    for(Crop r : crops) {
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
          plantCrop(x, y);
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
    for(int i = 0; i < crops.size(); i++) {
      if (crops[i].status == 2 && crops[i].x == x && crops[i].y == y) {
		switch(crops[i].type) {
		  case Crop::Type::Radish:
			gameAlert.setMessage("Got radish!");
			harvestedCrops.radishes++;
			break;
		  case Crop::Type::Eggplant:
			gameAlert.setMessage("Got eggplant!");
			harvestedCrops.eggplant++;
			break;
		  case Crop::Type::Corn:
			gameAlert.setMessage("Got corn!");
			harvestedCrops.corn++;
			break;
		}

        crops.erase(crops.begin() + i);
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

    for(int i = 0; i < crops.size(); i++) {
      if (crops[i].x == x && crops[i].y == y && crops[i].status < 2) {
        crops[i].status++;
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

  Crop::Type randomCropType() {
	int rindex = rand() % 3; // 3 total crops
	switch (rindex) {
	  case 0:
		return Crop::Type::Radish;
		break;
	  case 1:
		return Crop::Type::Eggplant;
		break;
	  case 2:
		return Crop::Type::Corn;
		break;
	}
   return Crop::Type::Radish;
}

  void plantCrop(int x, int y) {
	if(inventoryPtr->numSeeds < 1) {
      gameAlert.setMessage("No seeds!");
	  return;
	}

    for(Crop r : crops) {
      if (r.x == x && r.y == y) {
        return; // since there's already a radish here
      }
    }

    // create a new radish
    Crop r;
    r.x = x;
    r.y = y;
    r.status = 0;
    r.type = randomCropType();
    crops.push_back(r);
    gameAudio.playSeedSound();
	inventoryPtr->numSeeds--;
  }

  void spawnRock(int x, int y) {
    Rock r;
    r.x = x;
    r.y = y;
    rocks.push_back(r);
  }

  // redundant inner loop, I know, but this will be low-n & not run often
  void handleNewDay() {
    int x;
	int y;
	for(int i = 0; i < plots.size(); i++) {
	  if (plots[i].watered) {
		plots[i].watered = false;
        x = plots[i].x;
		y = plots[i].y;
		for(int i = 0; i < crops.size(); i++) {
		  if (crops[i].x == x && crops[i].y == y && crops[i].status < 2) {
			crops[i].status++;
		  }
		}
	  }
    }
  }
};

