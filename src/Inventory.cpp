enum Weekday { Monday, Tuesday, Wednesday, Thursday, Friday };

string weekdayString(Weekday w) {
  switch(w) {
	case Monday:
	  return "Monday";
	  break;
	case Tuesday:
	  return "Tuesday";
	  break;
	case Wednesday:
	  return "Wednesday";
	  break;
	case Thursday:
	  return "Thursday";
	  break;
	case Friday:
	  return "Friday";
	  break;
  } 
}

struct HarvestedCrops {
  int radishes;
  int eggplant;
  int corn;
};

struct Inventory {
  enum Item { Nothing, Fertilizer, Seeds, Hoe, WateringCan };

  sf::Texture texture;
  sf::Sprite sprite;
  sf::Texture cropBannerTexture;
  sf::Sprite cropBannerSprite;
  sf::Texture dayBannerTexture;
  sf::Sprite dayBannerSprite;
  sf::Texture selectedTexture;
  sf::Sprite selectedSprite;
  sf::Text inventoryText;
  sf::Font font;
  Item selectedItem;
  HarvestedCrops harvestedCrops;
  int numSeeds;
  int numFertilizer;
  Weekday weekday;

  void loadTexture() {
	weekday = Monday;
    numSeeds = 10;
    numFertilizer = 5;

    font.loadFromFile("./assets/PressStart2P.ttf");
    inventoryText.setFillColor(sf::Color::Black);
    inventoryText.setFont(font);

    texture.loadFromFile("./assets/inventory-banner.png");
    sprite.setTexture(texture);
    sprite.setScale(SCALE);
    // display inventory centered, at bottom of screen
    sprite.setPosition(sf::Vector2f((WORLD_WIDTH / 2) - (36 * 5), WORLD_HEIGHT - (36 * 5)));
 
    cropBannerTexture.loadFromFile("./assets/crop-banner.png");
    cropBannerSprite.setTexture(cropBannerTexture);
    cropBannerSprite.setScale(SCALE);
    cropBannerSprite.setPosition(sf::Vector2f(10, WORLD_HEIGHT/2 - 300));

    dayBannerTexture.loadFromFile("./assets/day-banner.png");
    dayBannerSprite.setTexture(dayBannerTexture);
    dayBannerSprite.setScale(SCALE);
    dayBannerSprite.setPosition(sf::Vector2f(WORLD_WIDTH/2 - (36*5), 10));

    selectedTexture.loadFromFile("./assets/selectedtile.png");
    selectedSprite.setTexture(selectedTexture);
    selectedSprite.setScale(SCALE);

	setSelectedTexture();
  }

  void drawInto(sf::RenderWindow* w) {
    w->draw(sprite);
    w->draw(selectedSprite);
    // draw fertilizer count
    inventoryText.setString(std::to_string(numFertilizer));
    inventoryText.setPosition(sf::Vector2f((WORLD_WIDTH / 2) + (40 * 5), WORLD_HEIGHT - (8 * 5)));
	w->draw(inventoryText);
	// seed count
    inventoryText.setString(std::to_string(numSeeds));
    inventoryText.setPosition(sf::Vector2f((WORLD_WIDTH / 2) - (5 * 5), WORLD_HEIGHT - (8 * 5)));
	w->draw(inventoryText);
    // harvested crops
	w->draw(cropBannerSprite);

    inventoryText.setString(std::to_string(harvestedCrops.radishes));
    inventoryText.setPosition(sf::Vector2f(110, WORLD_HEIGHT/2 - 180));
	w->draw(inventoryText);
    inventoryText.setString(std::to_string(harvestedCrops.corn));
    inventoryText.setPosition(sf::Vector2f(110, WORLD_HEIGHT/2 - 65));
	w->draw(inventoryText);
    inventoryText.setString(std::to_string(harvestedCrops.eggplant));
    inventoryText.setPosition(sf::Vector2f(110, WORLD_HEIGHT/2 + 45 ));
	w->draw(inventoryText);

    inventoryText.setString(weekdayString(weekday));
    inventoryText.setPosition(sf::Vector2f(WORLD_WIDTH/2 - 50, 75 ));
    w->draw(dayBannerSprite);
	w->draw(inventoryText);
  }

  void handleNewDay() {
	switch(weekday) {
	  case Monday:
        weekday = Tuesday;
		break;
	  case Tuesday:
        weekday = Wednesday;
		break;
	  case Wednesday:
        weekday = Thursday;
		break;
	  case Thursday:
        weekday = Friday;
		break;
	  case Friday:
        weekday = Monday;
		break;
	} 
  }

  void setSelectedTexture() {
    int pX;
    switch(selectedItem) {
      case(Nothing):
        pX = -999;
        break;
      case(Hoe):
        pX = 30;
        break;
      case(Seeds):
        pX = 10;
        break;
      case(Fertilizer):
        pX = -30;
        break;
      case(WateringCan):
        pX = -10;
        break;
      default:
        break;
    }
    selectedSprite.setPosition(sf::Vector2f((WORLD_WIDTH / 2) - (pX * 5), WORLD_HEIGHT - (30 * 5)));
  }

  void handleKeys() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      selectedItem = Hoe;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      selectedItem = Seeds;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
      selectedItem = WateringCan;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
      selectedItem = Fertilizer;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
      selectedItem = Nothing;
    }
    setSelectedTexture();
  }
};

